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

//! @file    ASch_Scheduler.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    22 Aug 2018
//!
//! @class   Scheduler
//! @brief   This is the scheduler module of ASch
//! 
//! The scheduler module is responsible of running tasks at given intervals based on system tick and run events reveiced
//! from the event module.

#ifndef ASCH_SCHEDULER_HPP_
#define ASCH_SCHEDULER_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <cstdint>
#include <ASch_Configuration.hpp>
#include <ASch_Queue.hpp>
#include <ASch_Queue.cpp>
#include <Hal_SysTick.hpp>
#include <Hal_Isr.hpp>

namespace ASch
{

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Structs and Enums
//-----------------------------------------------------------------------------------------------------------------------------

typedef void (*taskHandler_t)(void);
typedef void (*eventHandler_t)(void*);

typedef struct
{
    eventHandler_t Handler;
    void* pPayload;
} event_t;

typedef struct
{
    uint16_t intervalInMs;
    taskHandler_t Task;
} task_t;

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

void SysTickHandler(void);

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

/// @class Scheduler
class Scheduler
{
public:
    explicit Scheduler(Hal::SysTick& sysTickParameter, Hal::Isr& isrParameter, int16_t tickIntervalInMs);
    
    virtual void Start(void);
    virtual void Stop(void);

    virtual uint8_t GetTaskCount(void) const;
    virtual void CreateTask(task_t task);

private:
    //static void SysTickHandler(void);

    // Dependencies
    Hal::SysTick& sysTick;
    Hal::SysTick sysTickObject;
    Hal::Isr& isr;

    const uint8_t msPerTick;
    uint8_t taskCount;
    task_t tasks[schedulerTasksMax];
    int16_t taskMsCounters[schedulerTasksMax];

    Queue<event_t, schedulerEventsMax> eventQueue = Queue<event_t, schedulerEventsMax>();
};

} // namespace ASch

#endif // ASCH_SCHEDULER_HPP_