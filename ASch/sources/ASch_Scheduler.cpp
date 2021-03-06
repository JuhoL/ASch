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

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

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
Scheduler::taskState_t Scheduler::taskStates[] = {{.msCounter = 0U, .isRunning = false}};
volatile bool Scheduler::runTasks = false;
volatile bool Scheduler::runEvents = false;
uint8_t Scheduler::msPerTick = 0U;

SchedulerStatus Scheduler::status = SchedulerStatus::idle;

uint8_t Scheduler::taskCount = 0U;
task_t Scheduler::tasks[Config::schedulerTasksMax] = {{.intervalInMs = 0U, .Task = 0}};

Utils::Queue<event_t, Config::schedulerEventsMax> Scheduler::eventQueue = Utils::Queue<event_t, Config::schedulerEventsMax>();

uint8_t Scheduler::messageListenerCount = 0U;
messageListener_t Scheduler::messageListeners[Config::messageListenersMax] = {{.type = Message::invalid, .Handler = 0}};

//---------------------------------------
// Functions
//---------------------------------------
void Scheduler::Init(uint16_t tickIntervalInMs)
{
    if (status == SchedulerStatus::running)
    {
        ThrowError(SysError::accessNotPermitted);
    }
    else if (tickIntervalInMs == 0UL)
    {
        ThrowError(SysError::invalidParameters);
    }
    else
    {
        for (uint8_t i = 0U; i < Config::schedulerTasksMax; ++i)
        {
            tasks[i] = {.intervalInMs = 0U, .Task = 0};
            taskStates[i] = {.msCounter = 0U, .isRunning = false};
        }

        msPerTick = tickIntervalInMs;
        runTasks = false;
        runEvents = false;

        taskCount = 0U;
        eventQueue.Flush();
        messageListenerCount = 0U;
        status = SchedulerStatus::idle;

        Hal::SysTick::SetInterval(tickIntervalInMs);
        Hal::Isr::SetHandler(Hal::Interrupt::sysTick, Scheduler::TickHandler);
    }
    return;
}

void Scheduler::Start(void)
{
    Hal::Isr::Enable(Hal::Interrupt::sysTick);
    Hal::SysTick::Start();
    status = SchedulerStatus::running;
    return;
}

void Scheduler::Stop(void)
{
    Hal::SysTick::Stop();
    Hal::Isr::Disable(Hal::Interrupt::sysTick);
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
        Hal::Isr::DisableGlobal();
        if (taskCount < Config::schedulerTasksMax)
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
                taskStates[taskCount].msCounter = task.intervalInMs;
                taskStates[taskCount].isRunning = false;
                ++taskCount;
            }
        }
        else
        {
            ThrowError(SysError::insufficientResources);
        }
        Hal::Isr::EnableGlobal();
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

    if (taskId < Config::schedulerTasksMax)
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
        if (taskStates[taskId].isRunning == true)
        {
            taskStates[taskId].isRunning = false;
            tasks[taskId].Task();
        }
    }
    return;
}

void Scheduler::Sleep(void)
{
    Hal::System::Sleep();
    return;
}

void Scheduler::WakeUp(void)
{
    Hal::System::WakeUp();
    return;
}

void Scheduler::PushEvent(event_t const& event)
{
    if (event.Handler != 0)
    {
        Hal::Isr::DisableGlobal();
        bool errors = eventQueue.Push(event);

        if (errors == true)
        {
            ThrowError(SysError::insufficientResources);
        }
        else
        {
            runEvents = true;
            Hal::System::WakeUp();
        }
        Hal::Isr::EnableGlobal();
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
    if (messageListenerCount < Config::messageListenersMax)
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

void Scheduler::MainLoop(void)
{
    do
    {
        bool isIdle = true;
        if (runTasks == true)
        {
            runTasks = false;
            Scheduler::RunTasks();
            isIdle = false;
        }
        if (runEvents == true)
        {
            runEvents = false;
            Scheduler::RunEvents();
            isIdle = false;
        }

        // Enter sleep only after one idle run to ensure system is ready to sleep.
        if (isIdle == true)
        {
            Scheduler::Sleep();
        }
    } while (UNIT_TEST == 0);
    return;
}

void Scheduler::TickHandler(void)
{
    uint8_t taskCount = ASch::Scheduler::GetTaskCount();
    for (uint8_t taskId = 0U; taskId < taskCount; ++taskId)
    {
        if (taskStates[taskId].msCounter > msPerTick)
        {
            taskStates[taskId].msCounter -= msPerTick;
        }
        else
        {
            taskStates[taskId].msCounter = ASch::Scheduler::GetTaskInterval(taskId);
            taskStates[taskId].isRunning = true;
            runTasks = true;
        }
    }

    if (runTasks == true)
    {
        ASch::Scheduler::WakeUp();
    }
    return;
}

#if (UNIT_TEST == 1)
void Scheduler::Deinit(void)
{
    taskCount = 0U;
    eventQueue.Flush();
    messageListenerCount = 0U;
    status = SchedulerStatus::idle;
    return;
}
#endif

void Scheduler::ThrowError(SysError error)
{
    status = SchedulerStatus::error;
    System::Error(error);
}

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------

