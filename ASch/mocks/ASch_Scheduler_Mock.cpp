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

//! @file    Hal_Isr_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for ASch Scheduler.
//! 
//! These are mocks for ASch Scheduler utilising FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Isr_Mock.hpp>
#include <Hal_Isr.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Initialisation
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASchMock
{

Mock<Scheduler> mockASchScheduler;
static ASchMock::Scheduler& scheduler = mockASchScheduler.get();

void InitSystem(void)
{
    static bool isFirstInit = true;

    if (isFirstInit == true)
    {
        Fake(Method(mockASchScheduler, Start));
        Fake(Method(mockASchScheduler, Stop));
        Fake(Method(mockASchScheduler, GetStatus));
        Fake(Method(mockASchScheduler, GetTaskCount));
        Fake(Method(mockASchScheduler, CreateTask));
        Fake(Method(mockASchScheduler, DeleteTask));
        Fake(Method(mockASchScheduler, GetTaskInterval));
        Fake(Method(mockASchScheduler, RunTasks));
        Fake(Method(mockASchScheduler, Sleep));
        Fake(Method(mockASchScheduler, WakeUp));
        Fake(Method(mockASchScheduler, PushEvent));
        Fake(Method(mockASchScheduler, RunEvents));
        Fake(Method(mockASchScheduler, RegisterMessageListener));
        Fake(Method(mockASchScheduler, UnregisterMessageListener));
        Fake(Method(mockASchScheduler, GetNumberOfMessageListeners));
        Fake(Method(mockASchScheduler, PushMessage));
    }
    else
    {
        mockASchScheduler.Reset();
    }
    return;
}

} // namespace ASchMock

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

void Scheduler::Start(void)
{
    ASchMock::scheduler.Start();
    return;
}

void Scheduler::Stop(void)
{
    ASchMock::scheduler.Stop();
    return;
}

SchedulerStatus Scheduler::GetStatus(void)
{
    return ASchMock::scheduler.GetStatus();
}

uint8_t Scheduler::GetTaskCount(void)
{
    return ASchMock::scheduler.GetTaskCount();
}

void Scheduler::CreateTask(task_t task)
{
    ASchMock::scheduler.CreateTask(task);
    return;
}

void Scheduler::DeleteTask(taskHandler_t taskHandler)
{
    ASchMock::scheduler.DeleteTask(taskHandler);
    return;
}

uint16_t Scheduler::GetTaskInterval(uint8_t taskId)
{
    return ASchMock::scheduler.GetTaskInterval(taskId);
}

void Scheduler::RunTasks(void)
{
    ASchMock::scheduler.RunTasks();
    return;
}

void Scheduler::Sleep(void)
{
    ASchMock::scheduler.Sleep();
    return;
}

void Scheduler::WakeUp(void)
{
    ASchMock::scheduler.WakeUp();
    return;
}

void Scheduler::PushEvent(event_t const& event)
{
    ASchMock::scheduler.PushEvent(event);
    return;
}

void Scheduler::RunEvents(void)
{
    ASchMock::scheduler.RunEvents();
    return;
}

void Scheduler::RegisterMessageListener(messageListener_t const& listener)
{
    ASchMock::scheduler.RegisterMessageListener(listener);
    return;
}

void Scheduler::UnregisterMessageListener(messageListener_t const& listener)
{
    ASchMock::scheduler.UnregisterMessageListener(listener);
    return;
}

uint8_t Scheduler::GetNumberOfMessageListeners(Message type)
{
    return ASchMock::scheduler.GetNumberOfMessageListeners(type);
}

void Scheduler::PushMessage(message_t const& message)
{
    ASchMock::scheduler.PushMessage(message);
    return;
}


} // namespace Hal

