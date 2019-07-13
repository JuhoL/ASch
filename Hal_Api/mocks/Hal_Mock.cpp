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

//! @file    Hal_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for HAL classes.
//! 
//! These are initialisation functions for mocks. The mocks are utilising FakeIt framework.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Init Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace HalMock
{

void InitIsr(Mock<Hal::Isr>& mockIsr)
{
    Fake(Method(mockIsr, Init));
    Fake(Method(mockIsr, EnableGlobal));
    Fake(Method(mockIsr, DisableGlobal));
    Fake(Method(mockIsr, SetHandler));
    Fake(Method(mockIsr, GetHandler));
    Fake(Method(mockIsr, Enable));
    Fake(Method(mockIsr, Disable));
    Fake(Method(mockIsr, SetPriority));
    Fake(Method(mockIsr, GetPriority));
    Fake(Method(mockIsr, SetPending));
    Fake(Method(mockIsr, GetPending));
    Fake(Method(mockIsr, Clear));
    return;
}

void InitSysTick(Mock<Hal::SysTick>& mockSysTick)
{
    Fake(Method(mockSysTick, SetInterval));
    Fake(Method(mockSysTick, Start));
    Fake(Method(mockSysTick, Stop));
    Fake(Method(mockSysTick, IsRunning));
    return;
}

void InitGpio(Mock<Hal::Gpio>& mockGpio)
{
    Fake(Method(mockGpio, GetConfiguration));
    Fake(Method(mockGpio, SetConfiguration));
    Fake(Method(mockGpio, SetOutputState));
    Fake(Method(mockGpio, GetInputState));
    Fake(Method(mockGpio, GetOutputState));
    return;
}

} // namespace HalMock
