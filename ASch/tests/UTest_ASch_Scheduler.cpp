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

#include <ASch_Mock.hpp>
#include <Hal_Mock.hpp>

#include <ASch_Scheduler.hpp>
#include <ASch_Scheduler_Private.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

#define MOCK_SCHEDULER(interval)    ASch::Scheduler(mockSysTick.get(), mockIsr.get(), mockHalSystem.get(), mockSystem.get(), interval)

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

uint8_t criticalSystemErrorCount = 0U;

} // anonymous namespace

namespace Hal
{

void CriticalSystemError(void)
{
    ++criticalSystemErrorCount;
    return;
}

}


//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Developer starts or stops the scheduler", "[scheduler]")
{
    Mock<Hal::SysTick> mockSysTick;
    HalMock::InitSysTick(mockSysTick);

    Mock<Hal::Isr> mockIsr;
    HalMock::InitIsr(mockIsr);

    Mock<Hal::System> mockHalSystem;
    HalMock::InitSystem(mockHalSystem);

    Mock<ASch::System> mockSystem;
    ASchMock::InitSystem(mockSystem);

    GIVEN ("a scheduler is not yet created")
    {
        WHEN ("a scheduler is created")
        {
            ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

            THEN ("the scheduler shall configure system tick")
            {
                REQUIRE_PARAM_CALLS (1, mockSysTick, SetInterval, 1U);
            }
            AND_THEN ("scheduler tick handler shall be set as system tick handler")
            {
                REQUIRE_PARAM_CALLS (1, mockIsr, SetHandler, Hal::Interrupt::sysTick, Isr::Scheduler_SysTickHandler);
            }
            AND_THEN ("no tasks shall be running")
            {
                REQUIRE (scheduler.GetTaskCount() == 0U);
            }
            AND_THEN ("scheduler state shall be idle")
            {
                REQUIRE (scheduler.GetStatus() == ASch::SchedulerStatus::idle);
            }
        }
    }

    GIVEN ("a scheduler is created")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer starts the scheduler")
        {
            scheduler.Start();

            THEN ("SysTick ISR is enabled")
            {
                REQUIRE_PARAM_CALLS (1, mockIsr, Enable, Hal::Interrupt::sysTick);
            }
            AND_THEN ("SysTick is started")
            {
                REQUIRE_CALLS (1, mockSysTick, Start);
            }
            AND_THEN ("scheduler state shall be running")
            {
                REQUIRE (scheduler.GetStatus() == ASch::SchedulerStatus::running);
            }
        }
    }

    GIVEN ("a scheduler is running")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);
        scheduler.Start();

        WHEN ("developer stops the scheduler")
        {
            scheduler.Stop();

            THEN ("SysTick is stopped")
            {
                REQUIRE_CALLS(1, mockSysTick, Stop);
            }
            AND_THEN ("SysTick ISR is disabled") // ToDo: Verify order!
            {
                REQUIRE_PARAM_CALLS (1, mockIsr, Disable, Hal::Interrupt::sysTick);
            }
            AND_THEN ("scheduler state shall be stopped")
            {
                REQUIRE (scheduler.GetStatus() == ASch::SchedulerStatus::stopped);
            }
        }
    }
}

SCENARIO ("Developer configures scheduler wrong", "[scheduler]")
{
    Mock<Hal::SysTick> mockSysTick;
    HalMock::InitSysTick(mockSysTick);

    Mock<Hal::Isr> mockIsr;
    HalMock::InitIsr(mockIsr);

    Mock<Hal::System> mockHalSystem;
    HalMock::InitSystem(mockHalSystem);

    Mock<ASch::System> mockSystem;
    ASchMock::InitSystem(mockSystem);

    GIVEN ("a scheduler is not yet created")
    {
        WHEN ("a scheduler is created with interval of 0ms")
        {
            ASch::Scheduler scheduler = MOCK_SCHEDULER(0UL);
            (void)scheduler; // Get rid of unused variable warning.

            THEN ("a system error shall occur")
            {
                REQUIRE_PARAM_CALLS (1, mockSystem, Error, ASch::SysError::invalidParameters);
                REQUIRE (scheduler.GetStatus() == ASch::SchedulerStatus::error);
            }
        }
    }
    GIVEN ("a scheduler is not yet created")
    {
        WHEN ("a scheduler is created using the simple constructor")
        {
            ASch::Scheduler scheduler = ASch::Scheduler();
            (void)scheduler; // Get rid of unused variable warning.

            THEN ("a critical system error shall occur")
            {
                REQUIRE (criticalSystemErrorCount == 1U);
            }
        }
    }

    GIVEN ("a scheduler is created")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer creates another instance of scheduler")
        {
            ASch::Scheduler anotherScheduler = MOCK_SCHEDULER(1UL);

            THEN ("a system error shall occur")
            {
                REQUIRE_PARAM_CALLS (1, mockSystem, Error, ASch::SysError::multipleSchedulerInstances);
                REQUIRE (scheduler.GetStatus() == ASch::SchedulerStatus::error);
            }
        }
    }
}

SCENARIO ("Developer configures tasks successfully", "[scheduler]")
{
    Mock<Hal::SysTick> mockSysTick;
    HalMock::InitSysTick(mockSysTick);

    Mock<Hal::Isr> mockIsr;
    HalMock::InitIsr(mockIsr);

    Mock<Hal::System> mockHalSystem;
    HalMock::InitSystem(mockHalSystem);

    Mock<ASch::System> mockSystem;
    ASchMock::InitSystem(mockSystem);

    InitCallCounters();

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("a task (Task0) with interval of one is created")
        {
            scheduler.CreateTask({.intervalInMs = 1U, .Task = Handlers[0]});

            THEN ("global interrupts shall be disabled and again enabled")
            {
                REQUIRE_PARAM_CALLS (1, mockIsr, Disable, Hal::Interrupt::global);
                REQUIRE_PARAM_CALLS (1, mockIsr, Enable, Hal::Interrupt::global);
                REQUIRE_CALL_ORDER (CALL(mockIsr, Disable) + CALL(mockIsr, Enable));
            }
            AND_THEN ("the task count shall be one")
            {
                REQUIRE (scheduler.GetTaskCount() == 1U);
            }
            AND_THEN ("the task 0 interval will be set to one")
            {
                REQUIRE (scheduler.GetTaskInterval(0) == 1U);
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
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("a task (Task0) with interval three is created")
        {
            scheduler.CreateTask({.intervalInMs = 3U, .Task = Handlers[0]});

            THEN ("the interval of task ID 0 will be set to three")
            {
                REQUIRE (scheduler.GetTaskInterval(0) == 3U);
            }
            AND_WHEN ("another task (Task1) with interval of five is created")
            {
                scheduler.CreateTask({.intervalInMs = 5U, .Task = Handlers[1]});

                THEN ("the task count shall be two")
                {
                    REQUIRE (scheduler.GetTaskCount() == 2U);
                }
                AND_THEN ("the interval of task ID 1 will be set to five")
                {
                    REQUIRE (scheduler.GetTaskInterval(1) == 5U);
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
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        scheduler.CreateTask({.intervalInMs = 1U, .Task = Handlers[0]});
        scheduler.CreateTask({.intervalInMs = 2U, .Task = Handlers[1]});
        scheduler.CreateTask({.intervalInMs = 3U, .Task = Handlers[2]});

        WHEN ("the middle task, Task1 is deleted")
        {
            scheduler.DeleteTask(Handlers[1]);

            THEN ("the task count shall be two")
            {
                REQUIRE (scheduler.GetTaskCount() == 2U);
            }
            AND_THEN ("interval of task ID 0 shall be one")
            {
                REQUIRE (scheduler.GetTaskInterval(0) == 1U);
            }
            AND_THEN ("interval of task ID 1 shall be three")
            {
                REQUIRE (scheduler.GetTaskInterval(1) == 3U);
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
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer fills the scheduler up to task limit")
        {
            for (uint8_t i = 0U; i < ASch::schedulerTasksMax; ++i)
            {
                scheduler.CreateTask({.intervalInMs = 1U, .Task = Handlers[i]});
            }

            THEN ("task count shall be schedulerTasksMax")
            {
                REQUIRE (scheduler.GetTaskCount() == ASch::schedulerTasksMax);
            }
            AND_THEN ("no errors shall be triggered")
            {
                REQUIRE_CALLS (0, mockSystem, Error);
                REQUIRE (scheduler.GetStatus() != ASch::SchedulerStatus::error);
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer creates few tasks")
        {
            scheduler.CreateTask({.intervalInMs = 1U, .Task = Handlers[0]});
            scheduler.CreateTask({.intervalInMs = 1U, .Task = Handlers[1]});
            scheduler.CreateTask({.intervalInMs = 1U, .Task = Handlers[2]});

            AND_WHEN ("developer creates another task for an existing handler Task1")
            {
                scheduler.CreateTask({.intervalInMs = 2U, .Task = Handlers[1]});

                THEN ("task count shall stay as three")
                {
                    REQUIRE (scheduler.GetTaskCount() == 3U);
                }
                AND_THEN ("no errors shall be triggered")
                {
                    REQUIRE_CALLS (0, mockSystem, Error);
                    REQUIRE (scheduler.GetStatus() != ASch::SchedulerStatus::error);
                }
                AND_THEN ("Task1 time shall be updated")
                {
                    REQUIRE (scheduler.GetTaskInterval(1U) == 2U);
                }
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("a task (Task0) with interval three is created and SysTick triggers twice")
        {
            scheduler.CreateTask({.intervalInMs = 3U, .Task = Handlers[0]});
            Isr::Scheduler_SysTickHandler();
            Isr::Scheduler_SysTickHandler();

            THEN ("no wake up calls shall have been triggered")
            {
                REQUIRE_CALLS (0, mockHalSystem, WakeUp);

                AND_WHEN ("SysTick triggers one more time")
                {
                    Isr::Scheduler_SysTickHandler();

                    THEN ("wake up call shall occur")
                    {
                        REQUIRE_CALLS (1, mockHalSystem, WakeUp);

                        AND_WHEN ("main loop runs once")
                        {
                            ASch::SchedulerLoop();

                            THEN ("sleep shall not occur yet")
                            {
                                REQUIRE_CALLS (0, mockHalSystem, Sleep);

                                AND_WHEN ("main loops runs another time one idle run")
                                {
                                    ASch::SchedulerLoop();

                                    THEN ("sleep call shall occur without additional wakeup")
                                    {
                                        REQUIRE_CALLS (1, mockHalSystem, WakeUp);
                                        REQUIRE_CALLS (1, mockHalSystem, Sleep);
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
    Mock<Hal::SysTick> mockSysTick;
    HalMock::InitSysTick(mockSysTick);

    Mock<Hal::Isr> mockIsr;
    HalMock::InitIsr(mockIsr);

    Mock<Hal::System> mockHalSystem;
    HalMock::InitSystem(mockHalSystem);

    Mock<ASch::System> mockSystem;
    ASchMock::InitSystem(mockSystem);

    InitCallCounters();

    GIVEN ("the scheduler is running and task list is empty")
    {        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer tries to delete random task")
        {
            scheduler.DeleteTask(Handlers[0]);

            THEN ("task count shall stay zero")
            {
                REQUIRE (scheduler.GetTaskCount() == 0U);
            }
            AND_THEN ("no errors shall be triggered")
            {
                REQUIRE_CALLS (0, mockSystem, Error);
                REQUIRE (scheduler.GetStatus() != ASch::SchedulerStatus::error);
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer fills the scheduler task limit")
        {
            for (uint8_t i = 0U; i < ASch::schedulerTasksMax; ++i)
            {
                scheduler.CreateTask({.intervalInMs = 1U, .Task = Handlers[i]});
            }

            AND_WHEN ("developer tries to create another task")
            {
                scheduler.CreateTask({.intervalInMs = 1U, .Task = Handlers[ASch::schedulerTasksMax]});

                THEN ("system error shall trigger")
                {
                    REQUIRE_PARAM_CALLS (1, mockSystem, Error, ASch::SysError::insufficientResources);
                    REQUIRE (scheduler.GetStatus() == ASch::SchedulerStatus::error);
                }
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer tries to read task interval with an invalid ID")
        {
            uint16_t taskInterval = scheduler.GetTaskInterval(ASch::schedulerTasksMax);

            THEN ("the task interval will be set to zero")
            {
                REQUIRE (taskInterval == 0U);
            }
        }
    }
}

SCENARIO ("Developer pushes events successfully", "[scheduler]")
{
    Mock<Hal::SysTick> mockSysTick;
    HalMock::InitSysTick(mockSysTick);

    Mock<Hal::Isr> mockIsr;
    HalMock::InitIsr(mockIsr);

    Mock<Hal::System> mockHalSystem;
    HalMock::InitSystem(mockHalSystem);

    Mock<ASch::System> mockSystem;
    ASchMock::InitSystem(mockSystem);

    InitCallCounters();
    
    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("an event is pushed")
        {
            uint8_t testData0 = 0x12U;
            ASch::event_t testEvent = {.Handler = TestEventHandler0, .pPayload = static_cast<void*>(&testData0)};
            scheduler.PushEvent(testEvent);

            THEN ("global interrupts shall be disabled and again enabled")
            {
                REQUIRE_PARAM_CALLS (1, mockIsr, Disable, Hal::Interrupt::global);
                REQUIRE_PARAM_CALLS (1, mockIsr, Enable, Hal::Interrupt::global);
                REQUIRE_CALL_ORDER (CALL(mockIsr, Disable) + CALL(mockIsr, Enable));
            }
            AND_THEN ("wake up call shall occur")
            {
                REQUIRE_CALLS (1, mockHalSystem, WakeUp);

                AND_WHEN ("scheduler loop runs")
                {
                    ASch::SchedulerLoop();

                    THEN ("TestEventHandler shall be called once with testData0 pointer as payload")
                    {
                        REQUIRE (eventHandlerCalls[0] == 1U);
                        REQUIRE (pEventDatas[0] == static_cast<void*>(&testData0));
                    }
                    AND_WHEN ("sheduler loop runs another time")
                    {
                        ASch::SchedulerLoop();

                        THEN ("no other event runs shall occur")
                        {
                            REQUIRE (eventHandlerCalls[0] == 1U);
                        }
                        AND_THEN ("sleep call shall occur")
                        {
                            REQUIRE_CALLS (1, mockHalSystem, Sleep);
                        }
                    }
                }
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("three events are pushed and scheduler loop runs once")
        {
            uint8_t testData0 = 0x12U;
            ASch::event_t testEvent = {.Handler = TestEventHandler0, .pPayload = static_cast<void*>(&testData0)};
            scheduler.PushEvent(testEvent);

            uint8_t testData1 = 0x34U;
            testEvent = {.Handler = TestEventHandler1, .pPayload = static_cast<void*>(&testData1)};
            scheduler.PushEvent(testEvent);

            uint8_t testData2 = 0x56U;
            testEvent = {.Handler = TestEventHandler2, .pPayload = static_cast<void*>(&testData2)};
            scheduler.PushEvent(testEvent);

            ASch::SchedulerLoop();

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
                    scheduler.PushEvent(testEvent);

                    ASch::SchedulerLoop();

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
    Mock<Hal::SysTick> mockSysTick;
    HalMock::InitSysTick(mockSysTick);

    Mock<Hal::Isr> mockIsr;
    HalMock::InitIsr(mockIsr);

    Mock<Hal::System> mockHalSystem;
    HalMock::InitSystem(mockHalSystem);

    Mock<ASch::System> mockSystem;
    ASchMock::InitSystem(mockSystem);
    
    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("maximum number of events is pushed")
        {
            uint8_t testData0 = 0x12U;
            ASch::event_t testEvent = {.Handler = TestEventHandler0, .pPayload = static_cast<void*>(&testData0)};

            for (std::size_t i = 0; i < ASch::schedulerEventsMax; ++i)
            {
                scheduler.PushEvent(testEvent);
            }

            THEN ("no error shall be triggered")
            {
                REQUIRE_CALLS (0, mockSystem, Error);
                REQUIRE (scheduler.GetStatus() != ASch::SchedulerStatus::error);

                AND_WHEN ("another event is pushed")
                {
                    scheduler.PushEvent(testEvent);

                    THEN ("a system error shall occur")
                    {
                        REQUIRE_PARAM_CALLS (1, mockSystem, Error, ASch::SysError::insufficientResources);
                        REQUIRE (scheduler.GetStatus() == ASch::SchedulerStatus::error);
                    }
                }
            }
        }
    }
}

SCENARIO ("Developer manages message system successfully", "[scheduler]")
{
    uint8_t testData = 0x12U;
    
    Mock<Hal::SysTick> mockSysTick;
    HalMock::InitSysTick(mockSysTick);

    Mock<Hal::Isr> mockIsr;
    HalMock::InitIsr(mockIsr);

    Mock<Hal::System> mockHalSystem;
    HalMock::InitSystem(mockHalSystem);

    Mock<ASch::System> mockSystem;
    ASchMock::InitSystem(mockSystem);

    InitCallCounters();
    
    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer registers two message listeners for message_test_0 and one listener for message_test_1")
        {
            ASch::messageListener_t listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler0};
            scheduler.RegisterMessageListener(listener);

            listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler1};
            scheduler.RegisterMessageListener(listener);

            listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler2};
            scheduler.RegisterMessageListener(listener);

            THEN ("listener counts shall match with the registered listeners")
            {
                REQUIRE (scheduler.GetNumberOfMessageListeners(ASch::Message::test_0) == 2U);
                REQUIRE (scheduler.GetNumberOfMessageListeners(ASch::Message::test_1) == 1U);
            }
            AND_THEN ("no errors shall be triggered")
            {
                REQUIRE_CALLS (0, mockSystem, Error);
                REQUIRE (scheduler.GetStatus() != ASch::SchedulerStatus::error);

                AND_WHEN ("a message_test_0 is posted")
                {
                    scheduler.PushMessage({.type = ASch::Message::test_0, .pPayload = static_cast<void*>(&testData)});

                    THEN ("no errors shall be triggered")
                    {
                        REQUIRE_CALLS (0, mockSystem, Error);
                        REQUIRE (scheduler.GetStatus() != ASch::SchedulerStatus::error);
                    
                        AND_WHEN ("scheduler runs one cycle")
                        {
                            ASch::SchedulerLoop();

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
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        ASch::messageListener_t listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler0};
        scheduler.RegisterMessageListener(listener);

        listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler1};
        scheduler.RegisterMessageListener(listener);

        listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler2};
        scheduler.RegisterMessageListener(listener);

        WHEN ("developer unregisters listener for message_test_1")
        {
            listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler1};
            scheduler.UnregisterMessageListener(listener);

            THEN ("listener counts shall match with the registered listeners")
            {
                REQUIRE (scheduler.GetNumberOfMessageListeners(ASch::Message::test_0) == 2U);
                REQUIRE (scheduler.GetNumberOfMessageListeners(ASch::Message::test_1) == 0U);
            }
            AND_WHEN ("a message_test_1 is posted and scheduler runs one cycle")
            {
                scheduler.PushMessage({.type = ASch::Message::test_1, .pPayload = static_cast<void*>(&testData)});
                ASch::SchedulerLoop();

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
    
    Mock<Hal::SysTick> mockSysTick;
    HalMock::InitSysTick(mockSysTick);

    Mock<Hal::Isr> mockIsr;
    HalMock::InitIsr(mockIsr);

    Mock<Hal::System> mockHalSystem;
    HalMock::InitSystem(mockHalSystem);

    Mock<ASch::System> mockSystem;
    ASchMock::InitSystem(mockSystem);

    InitCallCounters();
    
    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer tries to register same listener twice")
        {
            ASch::messageListener_t listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler0};
            scheduler.RegisterMessageListener(listener);
            scheduler.RegisterMessageListener(listener);

            THEN ("the duplicate listener shall be ignored")
            {
                REQUIRE (scheduler.GetNumberOfMessageListeners(ASch::Message::test_0) == 1U);
            }
            AND_THEN ("no errors shall be triggered")
            {
                REQUIRE_CALLS (0, mockSystem, Error);
                REQUIRE (scheduler.GetStatus() != ASch::SchedulerStatus::error);

                AND_WHEN ("a message_test_0 is posted and scheduler runs one cycle")
                {
                    scheduler.PushMessage({.type = ASch::Message::test_0, .pPayload = static_cast<void*>(&testData)});
                    ASch::SchedulerLoop();

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
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        ASch::messageListener_t listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler0};
        scheduler.RegisterMessageListener(listener);

        listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler1};
        scheduler.RegisterMessageListener(listener);

        listener = {.type = ASch::Message::test_0, .Handler = TestEventHandler2};
        scheduler.RegisterMessageListener(listener);

        WHEN ("developer registers another listener")
        {
            listener = {.type = ASch::Message::test_1, .Handler = TestEventHandler2};
            scheduler.RegisterMessageListener(listener);

            THEN ("a system error shall occur")
            {
                REQUIRE_PARAM_CALLS (1, mockSystem, Error, ASch::SysError::insufficientResources);
                REQUIRE (scheduler.GetStatus() == ASch::SchedulerStatus::error);
            }
        }
    }
}

SCENARIO ("Developer fetches scheduler instance", "[scheduler]")
{
    Mock<Hal::SysTick> mockSysTick;
    HalMock::InitSysTick(mockSysTick);

    Mock<Hal::Isr> mockIsr;
    HalMock::InitIsr(mockIsr);

    Mock<Hal::System> mockHalSystem;
    HalMock::InitSystem(mockHalSystem);

    Mock<ASch::System> mockSystem;
    ASchMock::InitSystem(mockSystem);
    
    GIVEN ("the scheduler is running and task list is empty")
    {
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer requests scheduler instance pointer")
        {
            ASch::Scheduler* pScheduler = ASch::pGetSchedulerPointer();

            THEN ("the pointer shall match with the scheduler instance")
            {
                REQUIRE (pScheduler == &scheduler);
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
        Isr::Scheduler_SysTickHandler();
        ASch::SchedulerLoop();
    }
    return;
}

}
