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

//! @file    ASch_Scheduler_Mock.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for ASch System.
//! 
//! These are initialisation functions for mocks. The mocks are utilising FakeIt framework.

#ifndef ASCH_SCHEDULER_MOCK_HPP_
#define ASCH_SCHEDULER_MOCK_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Framework Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <catch.hpp>
#include <fakeit.hpp>
using namespace fakeit;

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Init Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASchMock
{

//! @class Scheduler
//! @brief This is a mock class for ASch Scheduler
class Scheduler
{
public:
    explicit Scheduler(void) {};
    virtual void Start(void);
    virtual void Stop(void);
    virtual SchedulerStatus GetStatus(void);
    virtual uint8_t GetTaskCount(void);
    virtual void CreateTask(task_t task);
    virtual void DeleteTask(taskHandler_t taskHandler);
    virtual uint16_t GetTaskInterval(uint8_t taskId);
    virtual void RunTasks(void);
    virtual void Sleep(void);
    virtual void WakeUp(void);
    virtual void PushEvent(event_t const& event);
    virtual void RunEvents(void);
    virtual void RegisterMessageListener(messageListener_t const& listener);
    virtual void UnregisterMessageListener(messageListener_t const& listener);
    virtual uint8_t GetNumberOfMessageListeners(Message type);
    virtual void PushMessage(message_t const& message);
};

extern Mock<Scheduler> mockASchScheduler;

/// @brief This function initialises the ASch Scheduler mock.
void InitScheduler(void);

} // namespace ASchMock

#endif // ASCH_SCHEDULER_MOCK_HPP_
