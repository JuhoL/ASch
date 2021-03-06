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
//! This class handles all the core functionality such as task scheduling, events and messages.

#ifndef ASCH_SCHEDULER_HPP_
#define ASCH_SCHEDULER_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>
#include <ASch_Configuration.hpp>
#include <Utils_Queue.hpp>
#include <ASch_System.hpp>
#include <Hal_SysTick.hpp>
#include <Hal_Isr.hpp>
#include <Hal_System.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

typedef void (*taskHandler_t)(void);            //!< A function pointer type for task handlers.
typedef void (*eventHandler_t)(const void*);    //!< A function pointer type for event handlers.
typedef void (*messageHandler_t)(const void*);  //!< A function pointer type for message handlers.

/// @brief This is an event struct that is used to push events into the scheduler.
typedef struct
{
    eventHandler_t Handler; //!< A function pointer to the event handler.
    const void* pPayload;   //!< A pointer to the optional payload.
} event_t;

/// @brief This is a task struct that is used to create tasks.
typedef struct
{
    uint16_t intervalInMs;  //!< Task interval in milliseconds.
    taskHandler_t Task;     //!< A function pointer for the task.
} task_t;

/// @brief This is a message listener struct that is used to create message listeners.
typedef struct
{
    Message type;               //!< Message type.
    messageHandler_t Handler;   //!< A function pointer to the message handler.
} messageListener_t;

/// @brief This is a message struct that is used to push messages.
typedef struct
{
    Message type;           //!< Message type.
    const void* pPayload;   //!< Pointer to the message payload.
} message_t;

/// @brief This is a scheduler status enum.
enum class SchedulerStatus
{
    idle = 0,   //!< Scheduler is idle and not started yet.
    running,    //!< Scheduler is started and is running.
    stopped,    //!< Scheduler has been stopped from running state.
    error       //!< Sheduler has encountered an error and cannot run.
};

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

//! @class Scheduler
//! @brief This is the scheduler module of ASch
//! This class handles all the core functionality such as task scheduling, events and messages.
//! - The tasks are functions that are called periodically.
//! - The events are used to trigger a function call outside current context. The typical use case for an event is passing data
//!   out from ISR context to the main handler within the same module. The event handler is always known and explicit and an event
//!   can have only a single handler. Events have higher priority than messages.
//! - The messages are used to inform other modules about certain events. The difference between event and message is that message
//!   can have multiple recipients and the transmitter does not know the recipients. This makes the message system the main
//!   communication channel between the modules.
class Scheduler
{
public:
    /// @brief Simple constructor.
    explicit Scheduler(void) {};

    /// @brief Initialises the scheduler.
    /// @param tickIntervalInMs - Interval of SysTick in milliseconds.
    static void Init(uint16_t tickIntervalInMs);

    /// @brief This function starts the scheduler.
    static void Start(void);

    /// @brief This function stops the scheduler.
    static void Stop(void);

    /// @brief This function returns the current status of the scheduler.
    /// @return Scheduler status
    static SchedulerStatus GetStatus(void);

    /// @brief This function returns the current task count.
    /// @return Current task count.
    static uint8_t GetTaskCount(void);

    /// @brief This function creates a given task.
    /// @param task - Task configuration struct.
    static void CreateTask(task_t task);

    /// @brief This function deletes the given task based on task handler.
    /// @param taskHandler - Function pointer to the task handler to be removed.
    static void DeleteTask(taskHandler_t taskHandler);

    /// @brief This function return the task interval of the given task ID.
    /// @param taskId - Task ID
    /// @return Task interval in milliseconds.
    static uint16_t GetTaskInterval(uint8_t taskId);
    
    /// @brief This function runs all the pending tasks.
    static void RunTasks(void);

    /// @brief This function puts the system in sleep mode.
    static void Sleep(void);

    /// @brief This function wakes the system up from sleep mode. 
    static void WakeUp(void);

    /// @brief This function is used to push an event into the scheduler.
    /// @param event - A refernce to the event to be pushed.
    static void PushEvent(event_t const& event);

    /// @brief This function runs all the pending events.
    static void RunEvents(void);

    /// @brief This function is used to register a message listener.
    /// @param listener - A reference to the listener to be registered.
    static void RegisterMessageListener(messageListener_t const& listener);
    
    /// @brief This function is used to unregister a message listener.
    /// @param listener - A reference to the listener to be unregistered.
    static void UnregisterMessageListener(messageListener_t const& listener);

    /// @brief This function returns the current number of message listeners of given message type.
    /// @param type - The type of message.
    /// @return Number of listeners.
    static uint8_t GetNumberOfMessageListeners(Message type);
    
    /// @brief This function pushes a message into the scheduler.
    /// @param message - A reference to the message to be pushed.
    static void PushMessage(message_t const& message);

    /// @brief This function is called from the main loop.
    static void MainLoop(void);

#if (UNIT_TEST == 1)
    /// @brief This function handles scheduler tick.
    static void TickHandler(void);

    /// @brief This function is used to deinitialise the scheduler in unit tests.
    static void Deinit(void);
#endif

private:
#if (UNIT_TEST == 0)
    /// @brief This function handles scheduler tick.
    static void TickHandler(void);
#endif
    /// @brief This function throws a system error.
    /// @param error - Type of error that occurred.
    static void ThrowError(SysError error);

    /// @brief This is a struct that is used to keep track of task states.
    typedef struct
    {
        uint16_t msCounter;
        bool isRunning;
    } taskState_t;

    static taskState_t taskStates[Config::schedulerTasksMax]; //!< Task states.
    static volatile bool runTasks;  //!< An indication to run the tasks.
    static volatile bool runEvents; //!< An indication to run the events.
    static uint8_t msPerTick;       //!< How many ms is one tick.

    static SchedulerStatus status;  //!< Current scheduler status
    
    static uint8_t taskCount;                       //!< Current task count.
    static task_t tasks[Config::schedulerTasksMax]; //!< List of tasks limited by configuration variable schedulerTasksMax

    static Utils::Queue<event_t, Config::schedulerEventsMax> eventQueue;    //!< Event queue.

    static uint8_t messageListenerCount;                            //!< Current message listener total count.
    static messageListener_t messageListeners[Config::messageListenersMax]; //!< List of message listeners limited by a configuration variable messageListenersMax.
};

} // namespace ASch

#endif // ASCH_SCHEDULER_HPP_
