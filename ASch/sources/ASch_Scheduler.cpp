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

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

ASch::Scheduler* pScheduler = 0;
taskStatus_t taskStatuses[ASch::schedulerTasksMax];
uint8_t msPerTick = 0U;

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

Scheduler::Scheduler(Hal::SysTick& sysTickParameter, Hal::Isr& isrParameter, System& systemParameter, uint16_t tickIntervalInMs)
    : sysTick(sysTickParameter),
      isr(isrParameter),
      system(systemParameter),
      taskCount(0)
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
            taskStatuses[i] = {.msCounter = 0U, .run = false};
        }

        sysTick.SetInterval(tickIntervalInMs);
        isr.SetHandler(Hal::interrupt_sysTick, Isr::Scheduler_SysTickHandler);

        pScheduler = this;
        msPerTick = tickIntervalInMs;
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
        if (taskCount < schedulerTasksMax)
        {
            bool isDuplicate = false;
            for (uint8_t i = 0U; i < taskCount; ++i)
            {
                if (tasks[i].Task == task.Task)
                {
                    isDuplicate = true;
                    break;
                }
            }

            if (isDuplicate == false)
            {
                tasks[taskCount] = task;
                taskStatuses[taskCount].msCounter = task.intervalInMs;
                taskStatuses[taskCount].run = false;
                ++taskCount;
            }
        }
        else
        {
            system.Error(sysError_insufficientResources);
        }
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

void Scheduler::RunTask(uint8_t taskId) const
{
    if ((taskId < schedulerTasksMax) && (tasks[taskId].Task != 0))
    {
        tasks[taskId].Task();
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
    uint8_t taskCount = pScheduler->GetTaskCount();
    do
    {
        for (uint8_t taskId = 0U; taskId < taskCount; ++taskId)
        {
            if (taskStatuses[taskId].run == true)
            {
                taskStatuses[taskId].run = false;
                pScheduler->RunTask(taskId);
            }
        }
    } while (UNIT_TEST == 0);
    return;
}

}

namespace Isr
{

void Scheduler_SysTickHandler(void)
{
    if (pScheduler != 0)
    {
        uint8_t taskCount = pScheduler->GetTaskCount();
        for (uint8_t taskId = 0U; taskId < taskCount; ++taskId)
        {
            if (taskStatuses[taskId].msCounter > msPerTick)
            {
                taskStatuses[taskId].msCounter -= msPerTick;
            }
            else
            {
                taskStatuses[taskId].msCounter = pScheduler->GetTaskInterval(taskId);
                taskStatuses[taskId].run = true;
            }
        }
    }
    return;
}

} // namespace Hal

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------
