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

//! @file    Hal_SysTick_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for SysTick HAL.
//! 
//! These are mocks for SysTick HAL utilising FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_SysTick_Mock.hpp>
#include <Hal_SysTick.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Initialisation
//-----------------------------------------------------------------------------------------------------------------------------

namespace HalMock
{

Mock<SysTick> mockHalSysTick;
static HalMock::SysTick& sysTick = mockHalSysTick.get();

void InitSysTick(void)
{
    static bool isFirstInit = true;

    if (isFirstInit == true)
    {
        Fake(Method(mockHalSysTick, SetInterval));
        Fake(Method(mockHalSysTick, Start));
        Fake(Method(mockHalSysTick, Stop));
        Fake(Method(mockHalSysTick, IsRunning));

        isFirstInit = false;
    }
    else
    {
        mockHalSysTick.ClearInvocationHistory();
    }
    return;
}

} // namespace HalMock

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

void SysTick::SetInterval(uint16_t intervalIn01Ms)
{
    HalMock::sysTick.SetInterval(intervalIn01Ms);
    return;
}

void SysTick::Start(void)
{
    HalMock::sysTick.Start();
    return;
}

void SysTick::Stop(void)
{
    HalMock::sysTick.Stop();
    return;
}

bool SysTick::IsRunning(void)
{
    return HalMock::sysTick.IsRunning();
}

} // namespace Hal

