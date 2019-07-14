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

//! @file    Hal_Gpio_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for HAL GPIO.
//! 
//! These are mocks for HAL GPIO utilising FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Gpio_Mock.hpp>
#include <Hal_Gpio.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Initialisation
//-----------------------------------------------------------------------------------------------------------------------------

namespace HalMock
{

Mock<Gpio> mockHalGpio;
static HalMock::Gpio& gpio = mockHalGpio.get();

void InitSystem(void)
{
    static bool isFirstInit = true;

    if (isFirstInit == true)
    {
        Fake(Method(mockHalGpio, GetConfiguration));
        Fake(Method(mockHalGpio, SetConfiguration));
        Fake(Method(mockHalGpio, SetOutputState));
        Fake(Method(mockHalGpio, GetInputState));
        Fake(Method(mockHalGpio, GetOutputState));
    }
    else
    {
        mockHalGpio.Reset();
    }
    return;
}

} // namespace HalMock

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

void Gpio::GetConfiguration(gpioConfig_t& gpio)
{
    HalMock::gpio.GetConfiguration(gpio);
    return;
}

void Gpio::SetConfiguration(gpioConfig_t& gpio)
{
    HalMock::gpio.SetConfiguration(gpio);
    return;
}

void Gpio::SetOutputState(Pin_t& pin, bool state)
{
    HalMock::gpio.SetOutputState(pin, state);
    return;
}

bool Gpio::GetInputState(Pin_t& pin)
{
    return HalMock::gpio.GetInputState(pin);;
}

bool Gpio::GetOutputState(Pin_t& pin)
{
    return HalMock::gpio.GetOutputState(pin);
}

} // namespace Hal

