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

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <fakeit.hpp>
#include <Catch_Utils.hpp>
using namespace fakeit;

#include <ASch_Scheduler.hpp>
#include <ASch_Scheduler_Private.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

#define MOCK_SCHEDULER(interval)    ASch::Scheduler(mockSysTick.get(), mockIsr.get(), mockHalSystem.get(), mockSystem.get(), interval)

inline void InitSysTickMock(Mock<Hal::SysTick>& mockSysTick)
{
    Fake(Method(mockSysTick, SetInterval));
    Fake(Method(mockSysTick, Start));
    Fake(Method(mockSysTick, Stop));
    return;
}

inline void InitIsrMock(Mock<Hal::Isr>& mockIsr)
{
    Fake(Method(mockIsr, SetHandler));
    Fake(Method(mockIsr, Enable));
    Fake(Method(mockIsr, Disable));
    return;
}

inline void InitHalSystemMock(Mock<Hal::System>& mockHalSystem)
{
    Fake(Method(mockHalSystem, Sleep));
    Fake(Method(mockHalSystem, WakeUp));
    return;
}

inline void InitSystemMock(Mock<ASch::System>& mockSystem)
{
    Fake(Method(mockSystem, Error));
    return;
}

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

static void* pEventDatas[3] = {0};
static uint8_t eventHandlerCalls[3] = {0U};

static void TestEventHandler0(void* pPayload)
{
    pEventDatas[0] = pPayload;
    ++eventHandlerCalls[0];
    return;
}

static void TestEventHandler1(void* pPayload)
{
    pEventDatas[1] = pPayload;
    ++eventHandlerCalls[1];
    return;
}

static void TestEventHandler2(void* pPayload)
{
    pEventDatas[2] = pPayload;
    ++eventHandlerCalls[2];
    return;
}

static ASch::taskHandler_t Handlers[6] = {TestTask0, TestTask1, TestTask2, TestTask3, TestTask4, TestTask5};

inline void CreateTask(ASch::Scheduler& scheduler, uint8_t taskId, uint16_t interval)
{
    testTaskCalls[taskId] = 0U;
    scheduler.CreateTask({.intervalInMs = interval, .Task = Handlers[taskId]});
    return;
}

static void RunTicks(uint32_t ticks);

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Developer starts or stops the scheduler", "[scheduler]")
{
    GIVEN ("a scheduler is not yet created")
    {
        WHEN ("a scheduler is created")
        {
            Mock<Hal::SysTick> mockSysTick;
            InitSysTickMock(mockSysTick);

            Mock<Hal::Isr> mockIsr;
            InitIsrMock(mockIsr);

            Mock<Hal::System> mockHalSystem;
            InitHalSystemMock(mockHalSystem);

            Mock<ASch::System> mockSystem;
            InitSystemMock(mockSystem);

            ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

            THEN ("the scheduler shall configure system tick")
            {
                REQUIRE_PARAM_CALLS (1, mockSysTick, SetInterval, 1U);
            }
            AND_THEN ("scheduler tick handler shall be set as system tick handler")
            {
                REQUIRE_PARAM_CALLS (1, mockIsr, SetHandler, Hal::interrupt_sysTick, Isr::Scheduler_SysTickHandler);
            }
            AND_THEN ("no tasks shall be running")
            {
                REQUIRE (scheduler.GetTaskCount() == 0U);
            }
        }
    }

    GIVEN ("a scheduler is created")
    {
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer starts the scheduler")
        {
            scheduler.Start();

            THEN ("SysTick ISR is enabled")
            {
                REQUIRE_PARAM_CALLS (1, mockIsr, Enable, Hal::interrupt_sysTick);
            }
            AND_THEN ("SysTick is started")
            {
                REQUIRE_CALLS (1, mockSysTick, Start);
            }
        }
    }

    GIVEN ("a scheduler is running")
    {
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
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
                REQUIRE_PARAM_CALLS (1, mockIsr, Disable, Hal::interrupt_sysTick);
            }
        }
    }
}

SCENARIO ("Developer configures scheduler wrong", "[scheduler]")
{
    GIVEN ("a scheduler is not yet created")
    {
        WHEN ("a scheduler is created with interval of 0ms")
        {
            Mock<Hal::SysTick> mockSysTick;
            InitSysTickMock(mockSysTick);

            Mock<Hal::Isr> mockIsr;
            InitIsrMock(mockIsr);

            Mock<Hal::System> mockHalSystem;
            InitHalSystemMock(mockHalSystem);

            Mock<ASch::System> mockSystem;
            InitSystemMock(mockSystem);

            ASch::Scheduler scheduler = MOCK_SCHEDULER(0UL);
            (void)scheduler; // Get rid of unused variable warning.

            THEN ("a system error shall occur")
            {
                REQUIRE_PARAM_CALLS (1, mockSystem, Error, ASch::sysError_invalidParameters);
            }
        }
    }
}

SCENARIO ("Developer configures tasks successfully", "[scheduler]")
{
    GIVEN ("the scheduler is running and task list is empty")
    {
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("a task (Task0) with interval of one is created")
        {
            CreateTask(scheduler, 0U, 1U);
            THEN ("global interrupts shall be disabled and again enabled")
            {
                REQUIRE_PARAM_CALLS (1, mockIsr, Disable, Hal::interrupt_global);
                REQUIRE_PARAM_CALLS (1, mockIsr, Enable, Hal::interrupt_global);
                REQUIRE_CALL_ORDER (CALL(mockIsr, Disable) + CALL(mockIsr, Enable));
            }
            AND_THEN ("the task count shall be one")
            {
                REQUIRE (scheduler.GetTaskCount() == 1U);
            }
            AND_THEN ("the task interval will be set to one")
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
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("a task (Task0) with interval three is created")
        {
            CreateTask(scheduler, 0U, 3U);

            THEN ("the interval of task ID 0 will be set to three")
            {
                REQUIRE (scheduler.GetTaskInterval(0) == 3U);
            }
            AND_WHEN ("another task (Task1) with interval of five is created")
            {
                CreateTask(scheduler, 1U, 5U);

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
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        CreateTask(scheduler, 0U, 1U);
        CreateTask(scheduler, 1U, 2U);
        CreateTask(scheduler, 2U, 3U);

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
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer fills the scheduler up to task limit")
        {
            for (uint8_t i = 0U; i < ASch::schedulerTasksMax; ++i)
            {
                CreateTask(scheduler, i, 1U);
            }

            THEN ("task count shall be schedulerTasksMax")
            {
                REQUIRE (scheduler.GetTaskCount() == ASch::schedulerTasksMax);
            }
            AND_THEN ("no errors shall be triggered")
            {
                REQUIRE_CALLS (0, mockSystem, Error);
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer creates few tasks")
        {
            CreateTask(scheduler, 0U, 1U);
            CreateTask(scheduler, 1U, 1U);
            CreateTask(scheduler, 2U, 1U);

            AND_WHEN ("developer creates another task for an existing handler Task1")
            {
                CreateTask(scheduler, 1U, 2U);

                THEN ("task count shall stay as three")
                {
                    REQUIRE (scheduler.GetTaskCount() == 3U);
                }
                AND_THEN ("no errors shall be triggered")
                {
                    REQUIRE_CALLS (0, mockSystem, Error);
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
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("a task (Task0) with interval three is created and SysTick triggers twice")
        {
            CreateTask(scheduler, 0U, 3U);
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

SCENARIO ("Developer configures tasks wrong", "[scheduler]")
{
    GIVEN ("the scheduler is running and task list is empty")
    {
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
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
            }
        }
    }

    GIVEN ("the scheduler is running and task list is empty")
    {
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("developer fills the scheduler task limit")
        {
            for (uint8_t i = 0U; i < ASch::schedulerTasksMax; ++i)
            {
                CreateTask(scheduler, i, 1U);
            }

            AND_WHEN ("developer tries to create another task")
            {
                CreateTask(scheduler, ASch::schedulerTasksMax, 1U);

                THEN ("system error shall trigger")
                {
                    REQUIRE_PARAM_CALLS (1, mockSystem, Error, ASch::sysError_insufficientResources);
                }
            }
        }
    }
}

SCENARIO ("Developer pushes events successfully", "[scheduler]")
{
    GIVEN ("the scheduler is running and task list is empty")
    {
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("an event is pushed")
        {
            uint8_t testData0 = 0x12U;
            ASch::event_t testEvent = {.Handler = TestEventHandler0, .pPayload = static_cast<void*>(&testData0)};
            eventHandlerCalls[0] = 0U;
            scheduler.PushEvent(testEvent);

            THEN ("global interrupts shall be disabled and again enabled")
            {
                REQUIRE_PARAM_CALLS (1, mockIsr, Disable, Hal::interrupt_global);
                REQUIRE_PARAM_CALLS (1, mockIsr, Enable, Hal::interrupt_global);
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
        Mock<Hal::SysTick> mockSysTick;
        InitSysTickMock(mockSysTick);

        Mock<Hal::Isr> mockIsr;
        InitIsrMock(mockIsr);

        Mock<Hal::System> mockHalSystem;
        InitHalSystemMock(mockHalSystem);

        Mock<ASch::System> mockSystem;
        InitSystemMock(mockSystem);
        
        ASch::Scheduler scheduler = MOCK_SCHEDULER(1UL);

        WHEN ("three events are pushed and scheduler loop runs once")
        {
            uint8_t testData0 = 0x12U;
            ASch::event_t testEvent = {.Handler = TestEventHandler0, .pPayload = static_cast<void*>(&testData0)};
            eventHandlerCalls[0] = 0U;
            scheduler.PushEvent(testEvent);

            uint8_t testData1 = 0x34U;
            testEvent = {.Handler = TestEventHandler1, .pPayload = static_cast<void*>(&testData1)};
            eventHandlerCalls[1] = 0U;
            scheduler.PushEvent(testEvent);

            uint8_t testData2 = 0x56U;
            testEvent = {.Handler = TestEventHandler2, .pPayload = static_cast<void*>(&testData2)};
            eventHandlerCalls[2] = 0U;
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
