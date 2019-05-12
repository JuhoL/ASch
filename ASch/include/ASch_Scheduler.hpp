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

#include <Utils_Types.hpp>
#include <ASch_Configuration.hpp>
#include <ASch_Queue.hpp>
#include <ASch_System.hpp>
#include <Hal_SysTick.hpp>
#include <Hal_Isr.hpp>
#include <Hal_System.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

typedef void (*taskHandler_t)(void);
typedef void (*eventHandler_t)(const void*);
typedef void (*messageHandler_t)(const void*);

typedef struct
{
    eventHandler_t Handler;
    const void* pPayload;
} event_t;

typedef struct
{
    uint16_t intervalInMs;
    taskHandler_t Task;
} task_t;

typedef struct
{
    messageType_t type;
    messageHandler_t Handler;
} messageListener_t;

typedef struct
{
    messageType_t type;
    const void* pPayload;
} message_t;

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

void SchedulerLoop(void);

}

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

/// @class Scheduler
class Scheduler
{
public:
    explicit Scheduler(Hal::SysTick& sysTickParameter, Hal::Isr& isrParameter, Hal::System& halSystemParameter, System& systemParameter, uint16_t tickIntervalInMs);
    
    test_virtual void Start(void) const;
    test_virtual void Stop(void) const;

    test_virtual uint8_t GetTaskCount(void) const;
    test_virtual void CreateTask(task_t task);
    test_virtual void DeleteTask(taskHandler_t taskHandler);

    test_virtual uint16_t GetTaskInterval(uint8_t taskId) const;
    test_virtual void RunTasks(void) const;

    test_virtual void Sleep(void) const;
    test_virtual void WakeUp(void) const;

    test_virtual void PushEvent(event_t const& event);
    test_virtual void RunEvents(void);

    test_virtual void RegisterMessageListener(messageListener_t const& listener);
    test_virtual void UnregisterMessageListener(messageListener_t const& listener);
    test_virtual uint8_t GetNumberOfMessageListeners(messageType_t type);
    test_virtual void PushMessage(message_t const& message);

private:
    // Dependencies
    Hal::SysTick& sysTick;
    Hal::Isr& isr;
    Hal::System& halSystem;
    System& system;

    uint8_t taskCount;
    task_t tasks[schedulerTasksMax];

    Queue<event_t, schedulerEventsMax> eventQueue = Queue<event_t, schedulerEventsMax>();

    uint8_t messageListenerCount;
    messageListener_t messageListeners[messageListenersMax];
};

} // namespace ASch

#endif // ASCH_SCHEDULER_HPP_
