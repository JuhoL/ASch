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
//! @brief   This is a GPIO HAL interface.
//! 
//! This class handles GPIO configuration and IO access.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Gpio.hpp>
#include <stm32f4xx.h>
#include <Utils_Assert.hpp>
#include <Utils_Bit.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

GPIO_TypeDef* const paGpios[] =
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH,
    GPIOI,
    GPIOJ,
    GPIOK
};

const uint32_t clockEnableBits[] =
{
    RCC_AHB1ENR_GPIOAEN_Pos,
    RCC_AHB1ENR_GPIOBEN_Pos,
    RCC_AHB1ENR_GPIOCEN_Pos,
    RCC_AHB1ENR_GPIODEN_Pos,
    RCC_AHB1ENR_GPIOEEN_Pos,
    RCC_AHB1ENR_GPIOFEN_Pos,
    RCC_AHB1ENR_GPIOGEN_Pos,
    RCC_AHB1ENR_GPIOHEN_Pos,
    RCC_AHB1ENR_GPIOIEN_Pos,
    RCC_AHB1ENR_GPIOJEN_Pos,
    RCC_AHB1ENR_GPIOKEN_Pos
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
    ASSERT(gpio.pin.number <= 15U);

    gpio.mode = GetMode(gpio.pin);
    gpio.isOpenDrain = IsOpenDrain(gpio.pin);
    gpio.speed = GetSpeed(gpio.pin);
    gpio.pull = GetPull(gpio.pin);
    gpio.alternateFunction = GetAlternateFunction(gpio.pin);

    return;
}

void Gpio::SetConfiguration(gpioConfig_t& gpio)
{
    ASSERT(gpio.pin.number <= 15U);
    
    EnablePortClock(gpio.pin.port);
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
    ASSERT(pin.number <= 15U);
    if (state == true)
    {
        Utils::SetBit(GPIO(pin.port)->BSRR, pin.number, true);
    }
    else
    {
        Utils::SetBit(GPIO(pin.port)->BSRR, pin.number + bitClearOffset, true);
    }
    return;
}

bool Gpio::GetInputState(Pin_t& pin)
{
    ASSERT_RETVAL(pin.number <= 15U, false);
    return (Utils::GetBits(GPIO(pin.port)->IDR, pin.number, stateMask) != 0UL);
}

bool Gpio::GetOutputState(Pin_t& pin)
{
    ASSERT_RETVAL(pin.number <= 15U, false);
    return (Utils::GetBits(GPIO(pin.port)->ODR, pin.number, stateMask) != 0UL);
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
    Utils::SetBits(GPIO(pin.port)->MODER, (pin.number * bitsInMode), modeMask, static_cast<uint32_t>(mode));
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
    Utils::SetBits(GPIO(pin.port)->OSPEEDR, (pin.number * bitsInSpeed), speedMask, static_cast<uint32_t>(speed));
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
    Utils::SetBits(GPIO(pin.port)->PUPDR, (pin.number * bitsInPull), pullMask, static_cast<uint32_t>(pull));
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

void Gpio::EnablePortClock(Port port)
{
    Utils::SetBit(RCC->AHB1ENR, clockEnableBits[static_cast<uint32_t>(port)], true);
    return;
}

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------

