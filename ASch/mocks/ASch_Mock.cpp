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

//! @file    ASch_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for ASch classes.
//! 
//! These are initialisation functions for mocks. The mocks are utilising FakeIt framework.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <ASch_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Init Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASchMock
{

void InitScheduler(Mock<ASch::Scheduler>& mockScheduler)
{
    Fake(Method(mockScheduler, Start));
    Fake(Method(mockScheduler, Stop));
    Fake(Method(mockScheduler, GetStatus));
    Fake(Method(mockScheduler, GetTaskCount));
    Fake(Method(mockScheduler, CreateTask));
    Fake(Method(mockScheduler, DeleteTask));
    Fake(Method(mockScheduler, GetTaskInterval));
    Fake(Method(mockScheduler, RunTasks));
    Fake(Method(mockScheduler, Sleep));
    Fake(Method(mockScheduler, WakeUp));
    Fake(Method(mockScheduler, PushEvent));
    Fake(Method(mockScheduler, RunEvents));
    Fake(Method(mockScheduler, RegisterMessageListener));
    Fake(Method(mockScheduler, UnregisterMessageListener));
    Fake(Method(mockScheduler, GetNumberOfMessageListeners));
    Fake(Method(mockScheduler, PushMessage));
    return;
}

void InitSystem(Mock<ASch::System>& mockSystem)
{
    Fake(Method(mockSystem, Error));
    Fake(Method(mockSystem, Init));
    Fake(Method(mockSystem, PreStartConfig));
    Fake(Method(mockSystem, PostStartConfig));
    return;
}

} // namespace ASchMock
