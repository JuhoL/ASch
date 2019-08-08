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

//! @file    Hal_Clocks_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for HAL GPIO.
//! 
//! These are mocks for HAL GPIO utilising FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Clocks_Mock.hpp>
#include <Hal_Clocks.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Initialisation
//-----------------------------------------------------------------------------------------------------------------------------

namespace HalMock
{

Mock<Clocks> mockHalClocks;
static HalMock::Clocks& clocks = mockHalClocks.get();

void InitClocks(void)
{
    static bool isFirstInit = true;

    if (isFirstInit == true)
    {
        Fake(Method(mockHalClocks, Enable));
        Fake(Method(mockHalClocks, Disable));
        Fake(Method(mockHalClocks, ConfigurePll));
        Fake(Method(mockHalClocks, IsRunning));
        Fake(Method(mockHalClocks, GetSysClockFrequency));
        Fake(Method(mockHalClocks, SetSysClockSource));
        Fake(Method(mockHalClocks, GetSysClockSource));

        isFirstInit = false;
    }
    else
    {
        mockHalClocks.Reset();
    }
    return;
}

} // namespace HalMock

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

Error Clocks::Enable(Hal::OscillatorType type)
{
    return HalMock::clocks.Enable(type);
}

Error Clocks::Disable(Hal::OscillatorType type)
{
    return HalMock::clocks.Disable(type);
}

Error Clocks::ConfigurePll(OscillatorType source, uint32_t frequency)
{
    return HalMock::clocks.ConfigurePll(source, frequency);
}

bool Clocks::IsRunning(Hal::OscillatorType type)
{
    return HalMock::clocks.IsRunning(type);
}

uint32_t Clocks::GetSysClockFrequency(void)
{
    return HalMock::clocks.GetSysClockFrequency();
}

Error Clocks::SetSysClockSource(Hal::OscillatorType type)
{
    return HalMock::clocks.SetSysClockSource(type);
}

Hal::OscillatorType Clocks::GetSysClockSource(void)
{
    return HalMock::clocks.GetSysClockSource();
}


} // namespace Hal

