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

//! @file    UTest_ASch_Scheduler.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    22 Aug 2018
//! 
//! @brief   These are unit tests for ASch_Scheduler.cpp
//! 
//! These are unit tests for ASch_Scheduler.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------
#include <Catch_Utils.hpp>

#include <ASch_Scheduler.hpp>
#include <ASch_Scheduler_Private.hpp>

#include <ASch_System_Mock.hpp>
#include <Hal_SysTick_Mock.hpp>
#include <Hal_Isr_Mock.hpp>
#include <Hal_System_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

// ---------- Test tasks ----------
static uint16_t testTaskCalls[6] = {0U};

static void TestTask0(void)
{
    ++testTaskCalls[0];
    return;
}

static void TestTask1(void)
{
    ++testTaskCalls[1];
    return;
}

static void TestTask2(void)
{
    ++testTaskCalls[2];
    return;
}

static void TestTask3(void)
{
    ++testTaskCalls[3];
    return;
}

static void TestTask4(void)
{
    ++testTaskCalls[4];
    return;
}

static void TestTask5(void)
{
    ++testTaskCalls[5];
    return;
}

static const void* pEventDatas[3] = {0};
static uint8_t eventHandlerCalls[3] = {0U};

static void TestEventHandler0(const void* pPayload)
{
    pEventDatas[0] = pPayload;
    ++eventHandlerCalls[0];
    return;
}

static void TestEventHandler1(const void* pPayload)
{
    pEventDatas[1] = pPayload;
    ++eventHandlerCalls[1];
    return;
}

static void TestEventHandler2(const void* pPayload)
{
    pEventDatas[2] = pPayload;
    ++eventHandlerCalls[2];
    return;
}

static ASch::taskHandler_t Handlers[6] = {TestTask0, TestTask1, TestTask2, TestTask3, TestTask4, TestTask5};


static void InitCallCounters(void)
{
    for (size_t i = 0; i < 6; ++i)
    {
        testTaskCalls[i] = 0U;
    }

    for (size_t i = 0; i < 3; ++i)
    {
        pEventDatas[i] = 0;
        eventHandlerCalls[i] = 0U;
    }
}

static void RunTicks(uint32_t ticks);

} // anonymous namespace


//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Developer starts or stops the scheduler", "[scheduler]")
{
    HalMock::InitIsr();
    HalMock::InitSysTick();
    ASch::Scheduler::Deinit();

    GIVEN ("a scheduler is not yet initialised")
    {
        WHEN ("a scheduler is initialised")
        {
            ASch::Scheduler::Init(1UL);

            THEN ("the scheduler shall configure system tick")
            {
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalSysTick, SetInterval, 1U);
            }
            AND_THEN ("scheduler tick handler shall be set as system tick handler")
            {
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalIsr, SetHandler, Hal::Interrupt::sysTick, ASch::Scheduler::TickHandler);
            }
            AND_THEN ("no tasks shall be running")
            {
                REQUIRE (ASch::Scheduler::GetTaskCount() == 0U);
            }
            AND_THEN ("scheduler state shall be idle")
            {
                REQUIRE (ASch::Scheduler::GetStatus() == ASch::SchedulerStatus::idle);
            }
        }
    }

    GIVEN ("a scheduler is initialised")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("developer starts the scheduler")
        {
            ASch::Scheduler::Start();

            THEN ("SysTick ISR is enabled")
            {
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalIsr, Enable, Hal::Interrupt::sysTick);
            }
            AND_THEN ("SysTick is started")
            {
                REQUIRE_CALLS (1, HalMock::mockHalSysTick, Start);
            }
            AND_THEN ("scheduler state shall be running")
            {
                REQUIRE (ASch::Scheduler::GetStatus() == ASch::SchedulerStatus::running);
            }
        }
    }

    GIVEN ("a scheduler is running")
    {
        ASch::Scheduler::Init(1UL);
        ASch::Scheduler::Start();

        WHEN ("developer stops the scheduler")
        {
            ASch::Scheduler::Stop();

            THEN ("SysTick is stopped")
            {
                REQUIRE_CALLS(1, HalMock::mockHalSysTick, Stop);
            }
            AND_THEN ("SysTick ISR is disabled") // ToDo: Verify order!
            {
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalIsr, Disable, Hal::Interrupt::sysTick);
            }
            AND_THEN ("scheduler state shall be stopped")
            {
                REQUIRE (ASch::Scheduler::GetStatus() == ASch::SchedulerStatus::stopped);
            }
        }
    }
}

SCENARIO ("Developer configures scheduler wrong", "[scheduler]")
{
    HalMock::InitIsr();
    HalMock::InitSysTick();
    ASchMock::InitSystem();
    ASch::Scheduler::Deinit();

    GIVEN ("a scheduler is not yet initialsied")
    {
        WHEN ("a scheduler is initialised with interval of 0ms")
        {
            ASch::Scheduler::Init(0UL);

            THEN ("a system error shall occur")
            {
                REQUIRE_PARAM_CALLS (1, ASchMock::mockASchSystem, Error, ASch::SysError::invalidParameters);
                REQUIRE (ASch::Scheduler::GetStatus() == ASch::SchedulerStatus::error);
            }
        }
    }

    GIVEN ("a scheduler is running")
    {
        ASch::Scheduler::Init(1UL);
        ASch::Scheduler::Start();

        WHEN ("developer reinitialises the scheduler")
        {
            ASch::Scheduler::Init(2UL);

            THEN ("a system error shall occur")
            {
                REQUIRE_PARAM_CALLS (1, ASchMock::mockASchSystem, Error, ASch::SysError::accessNotPermitted);
                REQUIRE (ASch::Scheduler::GetStatus() == ASch::SchedulerStatus::error);
            }
        }
    }
}

SCENARIO ("Developer configures tasks successfully", "[scheduler]")
{
    HalMock::InitIsr();
    HalMock::InitSystem();
    ASchMock::InitSystem();
    InitCallCounters();
    ASch::Scheduler::Deinit();

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("a task (Task0) with interval of one is created")
        {
            ASch::Scheduler::CreateTask({.intervalInMs = 1U, .Task = Handlers[0]});

            THEN ("global interrupts shall be disabled and again enabled")
            {
                REQUIRE_CALLS (1, HalMock::mockHalIsr, DisableGlobal);
                REQUIRE_CALLS (1, HalMock::mockHalIsr, EnableGlobal);
                REQUIRE_CALL_ORDER (CALL(HalMock::mockHalIsr, DisableGlobal) + CALL(HalMock::mockHalIsr, EnableGlobal));
            }
            AND_THEN ("the task count shall be one")
            {
                REQUIRE (ASch::Scheduler::GetTaskCount() == 1U);
            }
            AND_THEN ("the task 0 interval will be set to one")
            {
                REQUIRE (ASch::Scheduler::GetTaskInterval(0) == 1U);
            }
            AND_WHEN ("SysTick triggers")
            {
                RunTicks(1UL);

                THEN ("Task0 shall be called once")
                {
                    REQUIRE (testTaskCalls[0] == 1U);
                }
                AND_WHEN ("SysTick triggers five more times")
                {
                    RunTicks(5UL);

                    THEN ("Task0 shall be called five more additional times")
                    {
                        REQUIRE (testTaskCalls[0] == 6U);
                    }
                }
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("a task (Task0) with interval three is created")
        {
            ASch::Scheduler::CreateTask({.intervalInMs = 3U, .Task = Handlers[0]});

            THEN ("the interval of task ID 0 will be set to three")
            {
                REQUIRE (ASch::Scheduler::GetTaskInterval(0) == 3U);
            }
            AND_WHEN ("another task (Task1) with interval of five is created")
            {
                ASch::Scheduler::CreateTask({.intervalInMs = 5U, .Task = Handlers[1]});

                THEN ("the task count shall be two")
                {
                    REQUIRE (ASch::Scheduler::GetTaskCount() == 2U);
                }
                AND_THEN ("the interval of task ID 1 will be set to five")
                {
                    REQUIRE (ASch::Scheduler::GetTaskInterval(1) == 5U);
                }
                AND_WHEN ("SysTick triggers twice")
                {
                    RunTicks(2UL);

                    THEN ("no tasks shall run")
                    {
                        REQUIRE (testTaskCalls[0] == 0U);
                        REQUIRE (testTaskCalls[1] == 0U);
                    }
                    AND_WHEN ("SysTick triggers third time")
                    {
                        RunTicks(1UL);

                        THEN ("Task0 shall be called once")
                        {
                            REQUIRE (testTaskCalls[0] == 1U);
                            REQUIRE (testTaskCalls[1] == 0U);
                        }
                        AND_WHEN ("SysTick triggers one more times")
                        {
                            RunTicks(1UL);

                            THEN ("there shall be no new calls")
                            {
                                REQUIRE (testTaskCalls[0] == 1U);
                                REQUIRE (testTaskCalls[1] == 0U);
                            }
                            AND_WHEN ("SysTick triggers one more time")
                            {
                                RunTicks(1UL);

                                THEN ("Task1 shall be called once")
                                {
                                    REQUIRE (testTaskCalls[0] == 1U);
                                    REQUIRE (testTaskCalls[1] == 1U);
                                }
                                AND_WHEN ("SysTick triggers one more time")
                                {
                                    RunTicks(1UL);

                                    THEN ("Task0 shall be called once more")
                                    {
                                        REQUIRE (testTaskCalls[0] == 2U);
                                        REQUIRE (testTaskCalls[1] == 1U);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    GIVEN ("the scheduler is running and three tasks are created with different intervals")
    {
        ASch::Scheduler::Init(1UL);

        ASch::Scheduler::CreateTask({.intervalInMs = 1U, .Task = Handlers[0]});
        ASch::Scheduler::CreateTask({.intervalInMs = 2U, .Task = Handlers[1]});
        ASch::Scheduler::CreateTask({.intervalInMs = 3U, .Task = Handlers[2]});

        WHEN ("the middle task, Task1 is deleted")
        {
            ASch::Scheduler::DeleteTask(Handlers[1]);

            THEN ("the task count shall be two")
            {
                REQUIRE (ASch::Scheduler::GetTaskCount() == 2U);
            }
            AND_THEN ("interval of task ID 0 shall be one")
            {
                REQUIRE (ASch::Scheduler::GetTaskInterval(0) == 1U);
            }
            AND_THEN ("interval of task ID 1 shall be three")
            {
                REQUIRE (ASch::Scheduler::GetTaskInterval(1) == 3U);
            }
            AND_WHEN ("SysTick triggers three times")
            {
                RunTicks(3UL);

                THEN ("Task0 shall be called thrice")
                {
                    REQUIRE (testTaskCalls[0] == 3U);
                }
                AND_THEN ("Task1 shall not be called")
                {
                    REQUIRE (testTaskCalls[1] == 0U);
                }
                AND_THEN ("Task2 shall be called once")
                {
                    REQUIRE (testTaskCalls[2] == 1U);
                }
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("developer fills the scheduler up to task limit")
        {
            for (uint8_t i = 0U; i < ASch::Config::schedulerTasksMax; ++i)
            {
                ASch::Scheduler::CreateTask({.intervalInMs = 1U, .Task = Handlers[i]});
            }

            THEN ("task count shall be schedulerTasksMax")
            {
                REQUIRE (ASch::Scheduler::GetTaskCount() == ASch::Config::schedulerTasksMax);
            }
            AND_THEN ("no errors shall be triggered")
            {
                REQUIRE_CALLS (0, ASchMock::mockASchSystem, Error);
                REQUIRE (ASch::Scheduler::GetStatus() != ASch::SchedulerStatus::error);
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("developer creates few tasks")
        {
            ASch::Scheduler::CreateTask({.intervalInMs = 1U, .Task = Handlers[0]});
            ASch::Scheduler::CreateTask({.intervalInMs = 1U, .Task = Handlers[1]});
            ASch::Scheduler::CreateTask({.intervalInMs = 1U, .Task = Handlers[2]});

            AND_WHEN ("developer creates another task for an existing handler Task1")
            {
                ASch::Scheduler::CreateTask({.intervalInMs = 2U, .Task = Handlers[1]});

                THEN ("task count shall stay as three")
                {
                    REQUIRE (ASch::Scheduler::GetTaskCount() == 3U);
                }
                AND_THEN ("no errors shall be triggered")
                {
                    REQUIRE_CALLS (0, ASchMock::mockASchSystem, Error);
                    REQUIRE (ASch::Scheduler::GetStatus() != ASch::SchedulerStatus::error);
                }
                AND_THEN ("Task1 time shall be updated")
                {
                    REQUIRE (ASch::Scheduler::GetTaskInterval(1U) == 2U);
                }
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("a task (Task0) with interval three is created and SysTick triggers twice")
        {
            ASch::Scheduler::CreateTask({.intervalInMs = 3U, .Task = Handlers[0]});
            ASch::Scheduler::TickHandler();
            ASch::Scheduler::TickHandler();

            THEN ("no wake up calls shall have been triggered")
            {
                REQUIRE_CALLS (0, HalMock::mockHalSystem, WakeUp);

                AND_WHEN ("SysTick triggers one more time")
                {
                    ASch::Scheduler::TickHandler();

                    THEN ("wake up call shall occur")
                    {
                        REQUIRE_CALLS (1, HalMock::mockHalSystem, WakeUp);

                        AND_WHEN ("main loop runs once")
                        {
                            ASch::Scheduler::MainLoop();

                            THEN ("sleep shall not occur yet")
                            {
                                REQUIRE_CALLS (0, HalMock::mockHalSystem, Sleep);

                                AND_WHEN ("main loops runs another time one idle run")
                                {
                                    ASch::Scheduler::MainLoop();

                                    THEN ("sleep call shall occur without additional wakeup")
                                    {
                                        REQUIRE_CALLS (1, HalMock::mockHalSystem, WakeUp);
                                        REQUIRE_CALLS (1, HalMock::mockHalSystem, Sleep);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

SCENARIO ("Developer configures or uses tasks wrong", "[scheduler]")
{
    ASchMock::InitSystem();
    InitCallCounters();
    ASch::Scheduler::Deinit();

    GIVEN ("the scheduler is running and task list is empty")
    {        
        ASch::Scheduler::Init(1UL);

        WHEN ("developer tries to delete random task")
        {
            ASch::Scheduler::DeleteTask(Handlers[0]);

            THEN ("task count shall stay zero")
            {
                REQUIRE (ASch::Scheduler::GetTaskCount() == 0U);
            }
            AND_THEN ("no errors shall be triggered")
            {
                REQUIRE_CALLS (0, ASchMock::mockASchSystem, Error);
                REQUIRE (ASch::Scheduler::GetStatus() != ASch::SchedulerStatus::error);
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("developer fills the scheduler task limit")
        {
            for (uint8_t i = 0U; i < ASch::Config::schedulerTasksMax; ++i)
            {
                ASch::Scheduler::CreateTask({.intervalInMs = 1U, .Task = Handlers[i]});
            }

            AND_WHEN ("developer tries to create another task")
            {
                ASch::Scheduler::CreateTask({.intervalInMs = 1U, .Task = Handlers[ASch::Config::schedulerTasksMax]});

                THEN ("system error shall trigger")
                {
                    REQUIRE_PARAM_CALLS (1, ASchMock::mockASchSystem, Error, ASch::SysError::insufficientResources);
                    REQUIRE (ASch::Scheduler::GetStatus() == ASch::SchedulerStatus::error);
                }
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("developer tries to read task interval with an invalid ID")
        {
            uint16_t taskInterval = ASch::Scheduler::GetTaskInterval(ASch::Config::schedulerTasksMax);

            THEN ("the task interval will be set to zero")
            {
                REQUIRE (taskInterval == 0U);
            }
        }
    }
}

SCENARIO ("Developer pushes events successfully", "[scheduler]")
{
    HalMock::InitIsr();
    HalMock::InitSystem();
    InitCallCounters();
    ASch::Scheduler::Deinit();
    
    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("an event is pushed")
        {
            uint8_t testData0 = 0x12U;
            ASch::event_t testEvent = {.Handler = TestEventHandler0, .pPayload = static_cast<void*>(&testData0)};
            ASch::Scheduler::PushEvent(testEvent);

            THEN ("global interrupts shall be disabled and again enabled")
            {
                REQUIRE_CALLS (1, HalMock::mockHalIsr, DisableGlobal);
                REQUIRE_CALLS (1, HalMock::mockHalIsr, EnableGlobal);
                REQUIRE_CALL_ORDER (CALL(HalMock::mockHalIsr, DisableGlobal) + CALL(HalMock::mockHalIsr, EnableGlobal));
            }
            AND_THEN ("wake up call shall occur")
            {
                REQUIRE_CALLS (1, HalMock::mockHalSystem, WakeUp);

                AND_WHEN ("scheduler loop runs")
                {
                    ASch::Scheduler::MainLoop();

                    THEN ("TestEventHandler shall be called once with testData0 pointer as payload")
                    {
                        REQUIRE (eventHandlerCalls[0] == 1U);
                        REQUIRE (pEventDatas[0] == static_cast<void*>(&testData0));
                    }
                    AND_WHEN ("sheduler loop runs another time")
                    {
                        ASch::Scheduler::MainLoop();

                        THEN ("no other event runs shall occur")
                        {
                            REQUIRE (eventHandlerCalls[0] == 1U);
                        }
                        AND_THEN ("sleep call shall occur")
                        {
                            REQUIRE_CALLS (1, HalMock::mockHalSystem, Sleep);
                        }
                    }
                }
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("three events are pushed and scheduler loop runs once")
        {
            uint8_t testData0 = 0x12U;
            ASch::event_t testEvent = {.Handler = TestEventHandler0, .pPayload = static_cast<void*>(&testData0)};
            ASch::Scheduler::PushEvent(testEvent);

            uint8_t testData1 = 0x34U;
            testEvent = {.Handler = TestEventHandler1, .pPayload = static_cast<void*>(&testData1)};
            ASch::Scheduler::PushEvent(testEvent);

            uint8_t testData2 = 0x56U;
            testEvent = {.Handler = TestEventHandler2, .pPayload = static_cast<void*>(&testData2)};
            ASch::Scheduler::PushEvent(testEvent);

            ASch::Scheduler::MainLoop();

            THEN ("TestEventHandlers shall be called with testData pointers as payload")
            {
                REQUIRE (eventHandlerCalls[0] == 1U);
                REQUIRE (pEventDatas[0] == static_cast<void*>(&testData0));

                REQUIRE (eventHandlerCalls[1] == 1U);
                REQUIRE (pEventDatas[1] == static_cast<void*>(&testData1));

                REQUIRE (eventHandlerCalls[2] == 1U);
                REQUIRE (pEventDatas[2] == static_cast<void*>(&testData2));

                AND_WHEN ("test event 1 is pushed again with different payload and scheduler loop runs once")
                {
                    testEvent = {.Handler = TestEventHandler1, .pPayload = static_cast<void*>(&testData0)};
                    ASch::Scheduler::PushEvent(testEvent);

                    ASch::Scheduler::MainLoop();

                    THEN ("only TestEventHandler1 shall be called")
                    {
                        REQUIRE (eventHandlerCalls[0] == 1U);
                        REQUIRE (pEventDatas[0] == static_cast<void*>(&testData0));

                        REQUIRE (eventHandlerCalls[1] == 2U);
                        REQUIRE (pEventDatas[1] == static_cast<void*>(&testData0));

                        REQUIRE (eventHandlerCalls[2] == 1U);
                        REQUIRE (pEventDatas[2] == static_cast<void*>(&testData2));
                    }
                }
            }
        }
    }
}

SCENARIO ("Developer pushes events unsuccessfully", "[scheduler]")
{
    ASchMock::InitSystem();
    ASch::Scheduler::Deinit();

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("maximum number of events is pushed")
        {
            uint8_t testData0 = 0x12U;
            ASch::event_t testEvent = {.Handler = TestEventHandler0, .pPayload = static_cast<void*>(&testData0)};

            for (std::size_t i = 0; i < ASch::Config::schedulerEventsMax; ++i)
            {
                ASch::Scheduler::PushEvent(testEvent);
            }

            THEN ("no error shall be triggered")
            {
                REQUIRE_CALLS (0, ASchMock::mockASchSystem, Error);
                REQUIRE (ASch::Scheduler::GetStatus() != ASch::SchedulerStatus::error);

                AND_WHEN ("another event is pushed")
                {
                    ASch::Scheduler::PushEvent(testEvent);

                    THEN ("a system error shall occur")
                    {
                        REQUIRE_PARAM_CALLS (1, ASchMock::mockASchSystem, Error, ASch::SysError::insufficientResources);
                        REQUIRE (ASch::Scheduler::GetStatus() == ASch::SchedulerStatus::error);
                    }
                }
            }
        }
    }
}

SCENARIO ("Developer manages message system successfully", "[scheduler]")
{
    uint8_t testData = 0x12U;
    ASchMock::InitSystem();
    InitCallCounters();
    ASch::Scheduler::Deinit();
    
    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("developer registers two message listeners for message_test_0 and one listener for message_test_1")
        {
            ASch::messageListener_t listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler0};
            ASch::Scheduler::RegisterMessageListener(listener);

            listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler1};
            ASch::Scheduler::RegisterMessageListener(listener);

            listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler2};
            ASch::Scheduler::RegisterMessageListener(listener);

            THEN ("listener counts shall match with the registered listeners")
            {
                REQUIRE (ASch::Scheduler::GetNumberOfMessageListeners(ASch::Message::test_0) == 2U);
                REQUIRE (ASch::Scheduler::GetNumberOfMessageListeners(ASch::Message::test_1) == 1U);
            }
            AND_THEN ("no errors shall be triggered")
            {
                REQUIRE_CALLS (0, ASchMock::mockASchSystem, Error);
                REQUIRE (ASch::Scheduler::GetStatus() != ASch::SchedulerStatus::error);

                AND_WHEN ("a message_test_0 is posted")
                {
                    ASch::Scheduler::PushMessage({.type = ASch::Message::test_0, .pPayload = static_cast<void*>(&testData)});

                    THEN ("no errors shall be triggered")
                    {
                        REQUIRE_CALLS (0, ASchMock::mockASchSystem, Error);
                        REQUIRE (ASch::Scheduler::GetStatus() != ASch::SchedulerStatus::error);
                    
                        AND_WHEN ("scheduler runs one cycle")
                        {
                            ASch::Scheduler::MainLoop();

                            THEN ("message_test_0 listeners shall be called once with testData")
                            {
                                REQUIRE (eventHandlerCalls[0] == 1U);
                                REQUIRE (pEventDatas[0] == static_cast<void*>(&testData));
                                REQUIRE (eventHandlerCalls[2] == 1U);
                                REQUIRE (pEventDatas[2] == static_cast<void*>(&testData));
                            }
                            AND_THEN ("message_test_1 listener shall not be called")
                            {
                                REQUIRE (eventHandlerCalls[1] == 0U);
                            }
                        }
                    }
                }
            }
        }
    }

    GIVEN ("the scheduler is running, task list is empty, and there are two message listeners for message_test_0 and one listener for message_test_1")
    {
        ASch::Scheduler::Init(1UL);

        ASch::messageListener_t listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler0};
        ASch::Scheduler::RegisterMessageListener(listener);

        listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler1};
        ASch::Scheduler::RegisterMessageListener(listener);

        listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler2};
        ASch::Scheduler::RegisterMessageListener(listener);

        WHEN ("developer unregisters listener for message_test_1")
        {
            listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler1};
            ASch::Scheduler::UnregisterMessageListener(listener);

            THEN ("listener counts shall match with the registered listeners")
            {
                REQUIRE (ASch::Scheduler::GetNumberOfMessageListeners(ASch::Message::test_0) == 2U);
                REQUIRE (ASch::Scheduler::GetNumberOfMessageListeners(ASch::Message::test_1) == 0U);
            }
            AND_WHEN ("a message_test_1 is posted and scheduler runs one cycle")
            {
                ASch::Scheduler::PushMessage({.type = ASch::Message::test_1, .pPayload = static_cast<void*>(&testData)});
                ASch::Scheduler::MainLoop();

                THEN ("remvoed message_test_1 listeners shall not be called")
                {
                    REQUIRE (eventHandlerCalls[1] == 0U);
                }
            }
        }
    }
}

SCENARIO ("Developer manages message system unsuccessfully", "[scheduler]")
{
    uint8_t testData = 0x12U;
    ASchMock::InitSystem();
    InitCallCounters();
    ASch::Scheduler::Deinit();
    
    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler::Init(1UL);

        WHEN ("developer tries to register same listener twice")
        {
            ASch::messageListener_t listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler0};
            ASch::Scheduler::RegisterMessageListener(listener);
            ASch::Scheduler::RegisterMessageListener(listener);

            THEN ("the duplicate listener shall be ignored")
            {
                REQUIRE (ASch::Scheduler::GetNumberOfMessageListeners(ASch::Message::test_0) == 1U);
            }
            AND_THEN ("no errors shall be triggered")
            {
                REQUIRE_CALLS (0, ASchMock::mockASchSystem, Error);
                REQUIRE (ASch::Scheduler::GetStatus() != ASch::SchedulerStatus::error);

                AND_WHEN ("a message_test_0 is posted and scheduler runs one cycle")
                {
                    ASch::Scheduler::PushMessage({.type = ASch::Message::test_0, .pPayload = static_cast<void*>(&testData)});
                    ASch::Scheduler::MainLoop();

                    THEN ("the duplicated listener shall be called only once")
                    {
                        REQUIRE (eventHandlerCalls[0] == 1U);
                    }
                }
            }
        }
    }

    GIVEN ("the scheduler is running, task list is empty, and the message listener list is full")
    {
        ASch::Scheduler::Init(1UL);

        ASch::messageListener_t listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler0};
        ASch::Scheduler::RegisterMessageListener(listener);

        listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler1};
        ASch::Scheduler::RegisterMessageListener(listener);

        listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler2};
        ASch::Scheduler::RegisterMessageListener(listener);

        WHEN ("developer registers another listener")
        {
            listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler2};
            ASch::Scheduler::RegisterMessageListener(listener);

            THEN ("a system error shall occur")
            {
                REQUIRE_PARAM_CALLS (1, ASchMock::mockASchSystem, Error, ASch::SysError::insufficientResources);
                REQUIRE (ASch::Scheduler::GetStatus() == ASch::SchedulerStatus::error);
            }
        }
    }
}

namespace
{

static void RunTicks(uint32_t ticks)
{
    for (uint32_t i = 0UL; i < ticks; ++i)
    {
        ASch::Scheduler::TickHandler();
        ASch::Scheduler::MainLoop();
    }
    return;
}

}
