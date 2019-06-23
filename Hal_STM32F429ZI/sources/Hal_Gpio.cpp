//-----------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2019 Juho Lepistö
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

//! @file    Hal_Gpio.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    16 Jun 2019
//!
//! @class   Gpio
//! @brief   !!!!! Brief file description here !!!!!
//! 
//! !!!!! Detailed file description here !!!!!

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Gpio.hpp>
#include <stm32f4xx.h>
#include <Utils_Assert.hpp>
#include <Utils_Bit.hpp>
#include <Hal_Isr.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

GPIO_TypeDef* paGpios[] =
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG
};

const uint32_t bitsInMode = 2UL;
const uint32_t modeMask = 0x3UL;
const uint32_t bitsInSpeed = 2UL;
const uint32_t speedMask = 0x3UL;
const uint32_t bitsInPull = 2UL;
const uint32_t pullMask = 0x3UL;
const uint32_t stateMask = 0x1UL;
const uint32_t bitClearOffset = 16UL;
const uint32_t lockBitPosition = 16UL;

} // anonymous namespace

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{
#define GPIO(port)      paGpios[static_cast<uint32_t>(port)]
}

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Static Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 6. Class Member Definitions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

//---------------------------------------
// Public Functions
//---------------------------------------
Gpio::Gpio(void)
{
    return;
}

void Gpio::GetConfiguration(gpioConfig_t& gpio)
{
    Utils::Assert(gpio.pin.number <= 15U);

    gpio.mode = GetMode(gpio.pin);
    gpio.isOpenDrain = IsOpenDrain(gpio.pin);
    gpio.speed = GetSpeed(gpio.pin);
    gpio.pull = GetPull(gpio.pin);
    gpio.alternateFunction = GetAlternateFunction(gpio.pin);

    return;
}

void Gpio::SetConfiguration(gpioConfig_t& gpio)
{
    Utils::Assert(gpio.pin.number <= 15U);
    
    SetMode(gpio.pin, gpio.mode);
    SetOpenDrain(gpio.pin, gpio.isOpenDrain);
    SetSpeed(gpio.pin, gpio.speed);
    SetPull(gpio.pin, gpio.pull);
    SetAlternateFunction(gpio.pin, gpio.alternateFunction);
    
    return;
}

void Gpio::SetOutputState(Pin_t& pin, bool state)
{
    // See STM32F429ZI datasheet chapter 8.4.7.
    Utils::Assert(pin.number <= 15U);
    if (state == true)
    {
        GPIO(pin.port)->BSRR = Utils::SetBit(GPIO(pin.port)->BSRR, pin.number, true);
    }
    else
    {
        GPIO(pin.port)->BSRR = Utils::SetBit(GPIO(pin.port)->BSRR, pin.number + bitClearOffset, true);
    }
    return;
}

bool Gpio::GetInputState(Pin_t& pin)
{
    Utils::Assert(pin.number <= 15U);
    return (Utils::GetBits(GPIO(pin.port)->IDR, pin.number, stateMask) != 0UL);
}

bool Gpio::GetOutputState(Pin_t& pin)
{
    Utils::Assert(pin.number <= 15U);
    return (Utils::GetBits(GPIO(pin.port)->ODR, pin.number, stateMask) != 0UL);
}

void Gpio::LockPort(Port port)
{
    // See STM32F429ZI datasheet chapter 8.4.8.
    if (Utils::GetBit(GPIO(port)->LCKR, lockBitPosition) == false)
    {
#if 0
        Isr isr = Isr();
        isr.Disable(Interrupt::global);
        GPIO(port)->LCKR = Utils::SetBit(GPIO(port)->LCKR, lockBitPosition, true);
        GPIO(port)->LCKR = Utils::SetBit(GPIO(port)->LCKR, lockBitPosition, false);
        GPIO(port)->LCKR = Utils::SetBit(GPIO(port)->LCKR, lockBitPosition, true);
        isr.Enable(Interrupt::global);
#endif
    }
    return;
}

//---------------------------------------
// Private Functions
//---------------------------------------

GpioMode Gpio::GetMode(Pin_t& pin)
{
    // See STM32F429ZI datasheet chapter 8.4.1.
    uint32_t modeBits = Utils::GetBits(GPIO(pin.port)->MODER, (pin.number * bitsInMode), modeMask);
    return static_cast<GpioMode>(modeBits);
}

void Gpio::SetMode(Pin_t& pin, GpioMode mode)
{
    // See STM32F429ZI datasheet chapter 8.4.1.
    GPIO(pin.port)->MODER = Utils::SetBits(GPIO(pin.port)->MODER, (pin.number * bitsInMode), modeMask, static_cast<uint32_t>(mode));
    return;
}

bool Gpio::IsOpenDrain(Pin_t& pin)
{
    return Utils::GetBit(GPIO(pin.port)->OTYPER, pin.number);
}

void Gpio::SetOpenDrain(Pin_t& pin, bool isOpenDrain)
{
    Utils::SetBit(GPIO(pin.port)->OTYPER, pin.number, isOpenDrain);
    return;
}

GpioSpeed Gpio::GetSpeed(Pin_t& pin)
{
    // See STM32F429ZI datasheet chapter 8.4.3.
    uint32_t speedBits = Utils::GetBits(GPIO(pin.port)->OSPEEDR, (pin.number * bitsInSpeed), speedMask);
    return static_cast<GpioSpeed>(speedBits);
}

void Gpio::SetSpeed(Pin_t& pin, GpioSpeed speed)
{
    // See STM32F429ZI datasheet chapter 8.4.3.
    GPIO(pin.port)->OSPEEDR = Utils::SetBits(GPIO(pin.port)->OSPEEDR, (pin.number * bitsInSpeed), speedMask, static_cast<uint32_t>(speed));
    return;
}

GpioPull Gpio::GetPull(Pin_t& pin)
{
    // See STM32F429ZI datasheet chapter 8.4.4.
    uint32_t pullBits = Utils::GetBits(GPIO(pin.port)->PUPDR, (pin.number * bitsInPull), pullMask);
    return static_cast<GpioPull>(pullBits);
}

void Gpio::SetPull(Pin_t& pin, GpioPull pull)
{
    // See STM32F429ZI datasheet chapter 8.4.4.
    GPIO(pin.port)->PUPDR = Utils::SetBits(GPIO(pin.port)->PUPDR, (pin.number * bitsInPull), pullMask, static_cast<uint32_t>(pull));
    return;
}

AlternateFunction Gpio::GetAlternateFunction(Pin_t& pin)
{
    return AlternateFunction::sys1;
}

void Gpio::SetAlternateFunction(Pin_t& pin, AlternateFunction alternateFunction)
{
    return;
}

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------

