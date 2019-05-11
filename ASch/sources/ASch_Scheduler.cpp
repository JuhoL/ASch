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
} taskStatus_t;

typedef struct
{
    taskStatus_t taskStatuses[ASch::schedulerTasksMax];
    volatile bool runTasks;
    volatile bool runEvents;
    uint8_t msPerTick;
} schedulerStatus_t;

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

ASch::Scheduler* pScheduler = 0;
schedulerStatus_t schedulerStatus;

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

Scheduler::Scheduler(Hal::SysTick& sysTickParameter, Hal::Isr& isrParameter, Hal::System& halSystemParameter, System& systemParameter, uint16_t tickIntervalInMs)
    : sysTick(sysTickParameter),
      isr(isrParameter),
      halSystem(halSystemParameter),
      system(systemParameter),
      taskCount(0U),
      messageListenerCount(0U)
{
    if (tickIntervalInMs == 0UL)
    {
        system.Error(sysError_invalidParameters);
    }
    else
    {
        for (uint8_t i = 0U; i < schedulerTasksMax; ++i)
        {
            tasks[i] = {.intervalInMs = 0, .Task = 0};
            schedulerStatus.taskStatuses[i] = {.msCounter = 0U, .run = false};
        }

        schedulerStatus.msPerTick = tickIntervalInMs;
        schedulerStatus.runTasks = false;
        schedulerStatus.runEvents = false;

        sysTick.SetInterval(tickIntervalInMs);
        isr.SetHandler(Hal::interrupt_sysTick, Isr::Scheduler_SysTickHandler);

        pScheduler = this;
    }
    return;
}

void Scheduler::Start(void) const
{
    isr.Enable(Hal::interrupt_sysTick);
    sysTick.Start();
    return;
}

void Scheduler::Stop(void) const
{
    sysTick.Stop();
    isr.Disable(Hal::interrupt_sysTick);
    return;
}

uint8_t Scheduler::GetTaskCount(void) const
{
    return taskCount;
}

void Scheduler::CreateTask(task_t task)
{
    if ((task.Task != 0) && (task.intervalInMs > 0U))
    {
        isr.Disable(Hal::interrupt_global);
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
                schedulerStatus.taskStatuses[taskCount].msCounter = task.intervalInMs;
                schedulerStatus.taskStatuses[taskCount].run = false;
                ++taskCount;
            }
        }
        else
        {
            system.Error(sysError_insufficientResources);
        }
        isr.Enable(Hal::interrupt_global);
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

uint16_t Scheduler::GetTaskInterval(uint8_t taskId) const
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

void Scheduler::RunTasks(void) const
{
    for (uint8_t taskId = 0U; taskId < taskCount; ++taskId)
    {
        if (schedulerStatus.taskStatuses[taskId].run == true)
        {
            schedulerStatus.taskStatuses[taskId].run = false;
            tasks[taskId].Task();
        }
    }
    return;
}

void Scheduler::Sleep(void) const
{
    halSystem.Sleep();
    return;
}

void Scheduler::WakeUp(void) const
{
    halSystem.WakeUp();
    return;
}

void Scheduler::PushEvent(event_t const& event)
{
    if (event.Handler != 0)
    {
        isr.Disable(Hal::interrupt_global);
        bool errors = eventQueue.Push(event);

        if (errors == true)
        {
            system.Error(sysError_insufficientResources);
        }
        else
        {
            schedulerStatus.runEvents = true;
            halSystem.WakeUp();
        }
        isr.Enable(Hal::interrupt_global);
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

void Scheduler::RegisterMessageListener(messageListener_t listener)
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
        system.Error(sysError_insufficientResources);
    }
    return;
}

void Scheduler::UnregisterMessageListener(messageListener_t listener)
{
    bool isFound = false;
    for (uint8_t i = 0U; i < messageListenerCount; ++i)
    {
        if (isFound == false)
        {
            if (messageListeners[i].Handler == listener.Handler)
            {
                isFound = true;
            }
        }
        else
        {
            messageListeners[i - 1].Handler = messageListeners[i].Handler;
        }
    }
    if (isFound == true)
    {
        --messageListenerCount;
    }
    return;
}

void Scheduler::PostMessage(messageType_t type, void* pPayload)
{
    event_t event;
    for (uint8_t i = 0U; i < messageListenerCount; ++i)
    {
        if (messageListeners[i].type == type)
        {
            event.Handler = messageListeners[i].Handler;
            event.pPayload = pPayload;
            PushEvent(event);
        }
    }
    return;
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
        if (schedulerStatus.runTasks == true)
        {
            schedulerStatus.runTasks = false;
            pScheduler->RunTasks();
            isIdle = false;
        }
        if (schedulerStatus.runEvents == true)
        {
            schedulerStatus.runEvents = false;
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

}

namespace Isr
{

void Scheduler_SysTickHandler(void)
{
    uint8_t taskCount = pScheduler->GetTaskCount();
    for (uint8_t taskId = 0U; taskId < taskCount; ++taskId)
    {
        if (schedulerStatus.taskStatuses[taskId].msCounter > schedulerStatus.msPerTick)
        {
            schedulerStatus.taskStatuses[taskId].msCounter -= schedulerStatus.msPerTick;
        }
        else
        {
            schedulerStatus.taskStatuses[taskId].msCounter = pScheduler->GetTaskInterval(taskId);
            schedulerStatus.taskStatuses[taskId].run = true;
            schedulerStatus.runTasks = true;
        }
    }

    if (schedulerStatus.runTasks == true)
    {
        pScheduler->WakeUp();
    }
    return;
}

} // namespace Hal

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------

