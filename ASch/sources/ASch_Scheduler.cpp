//-----------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2018 Juho Lepistö
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without 
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------------------------------------------------------

//! @file    ASch_Scheduler.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    22 Aug 2018
//!
//! @class   Scheduler
//! @brief   This is the scheduler module of ASch
//! 
//! The scheduler module is responsible of running tasks at given intervals based on system tick and run events reveiced
//! from the event module.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <ASch_Scheduler.hpp>
#include <ASch_Scheduler_Private.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

typedef struct
{
    uint16_t msCounter;
    bool run;
} taskState_t;

typedef struct
{
    taskState_t taskStates[ASch::schedulerTasksMax];
    volatile bool runTasks;
    volatile bool runEvents;
    uint8_t msPerTick;
} schedulerState_t;

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

ASch::Scheduler* pScheduler = 0;
schedulerState_t schedulerState;

}

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Static Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 6. Class Member Definitions
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

//---------------------------------------
// Initialise static members
//---------------------------------------
Hal::SysTick* Scheduler::pSysTick = 0;
Hal::Isr* Scheduler::pIsr = 0;
Hal::System* Scheduler::pHalSystem = 0;
System* Scheduler::pSystem = 0;

SchedulerStatus Scheduler::status = SchedulerStatus::idle;

uint8_t Scheduler::taskCount = 0U;
task_t Scheduler::tasks[schedulerTasksMax] = {{.intervalInMs = 0, .Task = 0}};

Utils::Queue<event_t, schedulerEventsMax> Scheduler::eventQueue = Utils::Queue<event_t, schedulerEventsMax>();

uint8_t Scheduler::messageListenerCount = 0U;
messageListener_t Scheduler::messageListeners[messageListenersMax] = {{.type = Message::invalid, .Handler = 0}};

//---------------------------------------
// Functions
//---------------------------------------
Scheduler::Scheduler(Hal::SysTick& sysTickParameter, Hal::Isr& isrParameter, Hal::System& halSystemParameter, System& systemParameter, uint16_t tickIntervalInMs)
{
    pSysTick = &sysTickParameter;
    pIsr = &isrParameter;
    pHalSystem = &halSystemParameter;
    pSystem = &systemParameter;

    if (tickIntervalInMs == 0UL)
    {
        ThrowError(SysError::invalidParameters);
    }
    else if (pScheduler != 0)
    {
        ThrowError(SysError::multipleSchedulerInstances);
    }
    else
    {
        for (uint8_t i = 0U; i < schedulerTasksMax; ++i)
        {
            tasks[i] = {.intervalInMs = 0, .Task = 0};
            schedulerState.taskStates[i] = {.msCounter = 0U, .run = false};
        }

        schedulerState.msPerTick = tickIntervalInMs;
        schedulerState.runTasks = false;
        schedulerState.runEvents = false;

        pSysTick->SetInterval(tickIntervalInMs);
        pIsr->SetHandler(Hal::Interrupt::sysTick, Isr::Scheduler_SysTickHandler);

        pScheduler = this;
    }
    return;
}

Scheduler::Scheduler(void)
{
    if (pScheduler == 0)
    {
        Hal::CriticalSystemError();
    }
    return;
}

Scheduler::~Scheduler(void)
{
    InitStaticMembers();
    pScheduler = 0;

    return;
}

void Scheduler::Start(void)
{
    pIsr->Enable(Hal::Interrupt::sysTick);
    pSysTick->Start();
    status = SchedulerStatus::running;
    return;
}

void Scheduler::Stop(void)
{
    pSysTick->Stop();
    pIsr->Disable(Hal::Interrupt::sysTick);
    status = SchedulerStatus::stopped;
    return;
}

SchedulerStatus Scheduler::GetStatus(void)
{
    return status;
}

uint8_t Scheduler::GetTaskCount(void)
{
    return taskCount;
}

void Scheduler::CreateTask(task_t task)
{
    if ((task.Task != 0) && (task.intervalInMs > 0U))
    {
        pIsr->Disable(Hal::Interrupt::global);
        if (taskCount < schedulerTasksMax)
        {
            bool isDuplicate = false;
            for (uint8_t i = 0U; i < taskCount; ++i)
            {
                if (tasks[i].Task == task.Task)
                {
                    // In case of duplicates, just update the interval.
                    tasks[i].intervalInMs = task.intervalInMs;
                    isDuplicate = true;
                    break;
                }
            }

            if (isDuplicate == false)
            {
                tasks[taskCount] = task;
                schedulerState.taskStates[taskCount].msCounter = task.intervalInMs;
                schedulerState.taskStates[taskCount].run = false;
                ++taskCount;
            }
        }
        else
        {
            ThrowError(SysError::insufficientResources);
        }
        pIsr->Enable(Hal::Interrupt::global);
    }
    return;
}

void Scheduler::DeleteTask(taskHandler_t taskHandler)
{
    bool taskIsRemoved = false;
    for (uint8_t i = 0U; i < taskCount; ++i)
    {
        if (taskIsRemoved == false)
        {
            if (tasks[i].Task == taskHandler)
            {
                taskIsRemoved = true;
            }
        }
        else
        {
            tasks[i - 1] = tasks[i];
        }
    }

    if (taskIsRemoved == true)
    {
        --taskCount;
    }
    return;
}

uint16_t Scheduler::GetTaskInterval(uint8_t taskId)
{
    uint16_t interval;

    if (taskId < schedulerTasksMax)
    {
        interval = tasks[taskId].intervalInMs;
    }
    else
    {
        interval = 0UL;
    }

    return interval;
}

void Scheduler::RunTasks(void)
{
    for (uint8_t taskId = 0U; taskId < taskCount; ++taskId)
    {
        if (schedulerState.taskStates[taskId].run == true)
        {
            schedulerState.taskStates[taskId].run = false;
            tasks[taskId].Task();
        }
    }
    return;
}

void Scheduler::Sleep(void)
{
    pHalSystem->Sleep();
    return;
}

void Scheduler::WakeUp(void)
{
    pHalSystem->WakeUp();
    return;
}

void Scheduler::PushEvent(event_t const& event)
{
    if (event.Handler != 0)
    {
        pIsr->Disable(Hal::Interrupt::global);
        bool errors = eventQueue.Push(event);

        if (errors == true)
        {
            ThrowError(SysError::insufficientResources);
        }
        else
        {
            schedulerState.runEvents = true;
            pHalSystem->WakeUp();
        }
        pIsr->Enable(Hal::Interrupt::global);
    }
    return;
}

void Scheduler::RunEvents(void)
{
    while (eventQueue.GetNumberOfElements() > 0)
    {
        event_t event;
        eventQueue.Pop(event);
        event.Handler(event.pPayload);
    }
    return;
}

void Scheduler::RegisterMessageListener(messageListener_t const& listener)
{
    if (messageListenerCount < messageListenersMax)
    {
        if (listener.Handler != 0)
        {
            bool isDuplicate = false;
            for (uint8_t i = 0U; (i < messageListenerCount) && (isDuplicate == false); ++i)
            {
                isDuplicate = (messageListeners[i].Handler == listener.Handler);
            }

            if (isDuplicate == false)
            {
                messageListeners[messageListenerCount] = listener;
                ++messageListenerCount;
            }
        }
    }
    else
    {
        ThrowError(SysError::insufficientResources);
    }
    return;
}

void Scheduler::UnregisterMessageListener(messageListener_t const& listener)
{
    bool isFound = false;
    for (uint8_t i = 0U; i < messageListenerCount; ++i)
    {
        if (isFound == false)
        {
            if ((messageListeners[i].Handler == listener.Handler)
                && (messageListeners[i].type == listener.type))
            {
                isFound = true;
            }
        }
        else
        {
            messageListeners[i - 1U] = messageListeners[i];
        }
    }
    if (isFound == true)
    {
        --messageListenerCount;
    }
    return;
}

uint8_t Scheduler::GetNumberOfMessageListeners(Message type)
{
    uint8_t listeners = 0U;
    for (uint8_t i = 0U; i < messageListenerCount; ++i)
    {
        if (messageListeners[i].type == type)
        {
            ++listeners;
        }
    }
    return listeners;
}

void Scheduler::PushMessage(message_t const& message)
{
    event_t event;
    for (uint8_t i = 0U; i < messageListenerCount; ++i)
    {
        if (messageListeners[i].type == message.type)
        {
            event.Handler = messageListeners[i].Handler;
            event.pPayload = message.pPayload;
            PushEvent(event);
        }
    }
    return;
}

void Scheduler::InitStaticMembers(void)
{
    pSysTick = 0;
    pIsr = 0;
    pHalSystem = 0;
    pSystem = 0;
    taskCount = 0U;
    eventQueue = Utils::Queue<event_t, schedulerEventsMax>();
    messageListenerCount = 0U;
    status = SchedulerStatus::idle;

    return;
}

void Scheduler::ThrowError(SysError error)
{
    status = SchedulerStatus::error;
    pSystem->Error(error);
}

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

void SchedulerLoop(void)
{
    do
    {
        bool isIdle = true;
        if (schedulerState.runTasks == true)
        {
            schedulerState.runTasks = false;
            pScheduler->RunTasks();
            isIdle = false;
        }
        if (schedulerState.runEvents == true)
        {
            schedulerState.runEvents = false;
            pScheduler->RunEvents();
            isIdle = false;
        }

        // Enter sleep only after one idle run to ensure system is ready to sleep.
        if (isIdle == true)
        {
            pScheduler->Sleep();
        }
    } while (UNIT_TEST == 0);
    return;
}

Scheduler* pGetSchedulerPointer(void)
{
    return pScheduler;
}

}

namespace Isr
{

void Scheduler_SysTickHandler(void)
{
    uint8_t taskCount = pScheduler->GetTaskCount();
    for (uint8_t taskId = 0U; taskId < taskCount; ++taskId)
    {
        if (schedulerState.taskStates[taskId].msCounter > schedulerState.msPerTick)
        {
            schedulerState.taskStates[taskId].msCounter -= schedulerState.msPerTick;
        }
        else
        {
            schedulerState.taskStates[taskId].msCounter = pScheduler->GetTaskInterval(taskId);
            schedulerState.taskStates[taskId].run = true;
            schedulerState.runTasks = true;
        }
    }

    if (schedulerState.runTasks == true)
    {
        pScheduler->WakeUp();
    }
    return;
}

} // namespace Hal

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------

