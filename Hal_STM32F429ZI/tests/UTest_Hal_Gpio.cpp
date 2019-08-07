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

//! @file    UTest_Hal_Gpio.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    16 Jun 2019
//! 
//! @brief   These are unit tests for Hal_Gpio.cpp
//! 
//! These are unit tests for Hal_Gpio.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Catch_Utils.hpp>

#include <Hal_Gpio.hpp>
#include <Utils_Bit.hpp>
#include <stm32f429xx_mock.h>
#include <Utils_Assert_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Developer configures GPIO", "[gpio]")
{
    Hal_Mock::InitGpioRegisters();
    Hal_Mock::InitRccRegisters();

    GIVEN ("a GPIO class is created and GPIO configuration struct is created for PC5")
    {
        Hal::Gpio gpio = Hal::Gpio();
        Hal::gpioConfig_t gpioConfig =
        {
            .pin = {.port = Hal::Port::c, .number = 5UL},
            .mode = Hal::GpioMode::output,
            .isOpenDrain = true,
            .speed = Hal::GpioSpeed::high,
            .pull = Hal::GpioPull::pullUp,
            .alternateFunction = Hal::AlternateFunction::sys1
        };

        WHEN ("set configuration is called with the struct")
        {
            gpio.SetConfiguration(gpioConfig);

            THEN ("PC5 mode is set to 01b")
            {
                REQUIRE (GPIOC->MODER == 0x00000400UL);
            }
            AND_THEN ("PC5 is open drain")
            {
                REQUIRE (GPIOC->OTYPER == 0x00000020UL);
            }
            AND_THEN ("PC5 speed is 10b")
            {
                REQUIRE (GPIOC->OSPEEDR == 0x00000800UL);
            }
            AND_THEN ("PC5 pullup is set to 01b")
            {
                REQUIRE (GPIOC->PUPDR == 0x00000400UL);
            }
            AND_THEN ("PC5 alternate function is set to 00")
            {
                ; // ToDo: Add Alternate Function tests later.
            }
            AND_THEN ("Port C clock is enabled")
            {
                REQUIRE (Utils::GetBit(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN_Pos) == true);
            }
            AND_WHEN ("The configuration is read to an empty struct")
            {
                Hal::gpioConfig_t gpioConfig2 =
                {
                    .pin = {.port = Hal::Port::c, .number = 5UL}
                };
                gpio.GetConfiguration(gpioConfig2);

                THEN ("the structs shall be identical")
                {
                    REQUIRE (gpioConfig.mode == gpioConfig2.mode);
                    REQUIRE (gpioConfig.isOpenDrain == gpioConfig2.isOpenDrain);
                    REQUIRE (gpioConfig.speed == gpioConfig2.speed);
                    REQUIRE (gpioConfig.pull == gpioConfig2.pull);
                    REQUIRE (gpioConfig.alternateFunction == gpioConfig2.alternateFunction);
                }
            }
        }
    }

    GIVEN ("a GPIO class is created and empty GPIO configuration struct is created for PA13")
    {
        Hal::Gpio gpio = Hal::Gpio();
        Hal::gpioConfig_t gpioConfig =
        {
            .pin = {.port = Hal::Port::a, .number = 13UL}
        };

        // Change some of the struct values to later ensure that they are changed.
        gpioConfig.isOpenDrain = true;
        gpioConfig.pull = Hal::GpioPull::pullUp;

        WHEN ("get configuration is called with the struct")
        {
            gpio.GetConfiguration(gpioConfig);

            THEN ("struct mode is set to alternate function")
            {
                REQUIRE (gpioConfig.mode == Hal::GpioMode::alternate);
            }
            AND_THEN ("struct open drain is not active")
            {
                REQUIRE (gpioConfig.isOpenDrain == false);
            }
            AND_THEN ("struct speed is very high")
            {
                REQUIRE (gpioConfig.speed == Hal::GpioSpeed::veryHigh);
            }
            AND_THEN ("struct pullup is floating")
            {
                REQUIRE (gpioConfig.pull == Hal::GpioPull::floating);
            }
            AND_THEN ("PC5 alternate function is set to 00")
            {
                ; // ToDo: Add Alternate Function tests later.
            }
        }
    }
}

SCENARIO ("Developer configures GPIO wrong", "[gpio]")
{
    Hal_Mock::InitGpioRegisters();
    ASchMock::Assert::Init();

    GIVEN ("a GPIO class is created and GPIO configuration struct is created for PC16")
    {
        Hal::Gpio gpio = Hal::Gpio();
        Hal::gpioConfig_t gpioConfig =
        {
            .pin = {.port = Hal::Port::c, .number = 16UL},
            .mode = Hal::GpioMode::output,
            .isOpenDrain = true,
            .speed = Hal::GpioSpeed::high,
            .pull = Hal::GpioPull::pullUp,
            .alternateFunction = Hal::AlternateFunction::sys1
        };

        WHEN ("set configuration is called with the struct")
        {
            gpio.SetConfiguration(gpioConfig);

            THEN ("an assert failure shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }

    GIVEN ("a GPIO class is created and GPIO configuration struct is created for PA16")
    {
        Hal::Gpio gpio = Hal::Gpio();
        Hal::gpioConfig_t gpioConfig =
        {
            .pin = {.port = Hal::Port::a, .number = 16UL}
        };

        WHEN ("get configuration is called with the struct")
        {
            gpio.SetConfiguration(gpioConfig);

            THEN ("an assert failure shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }
}

SCENARIO ("Developer uses GPIO", "[gpio]")
{
    Hal_Mock::InitGpioRegisters();

    GIVEN ("PC5 is configured as an input")
    {
        Hal::Gpio gpio = Hal::Gpio();
        Hal::gpioConfig_t gpioConfig =
        {
            .pin = {.port = Hal::Port::c, .number = 5UL},
            .mode = Hal::GpioMode::input,
            .isOpenDrain = false,
            .speed = Hal::GpioSpeed::low,
            .pull = Hal::GpioPull::pullUp,
            .alternateFunction = Hal::AlternateFunction::sys1
        };
        gpio.SetConfiguration(gpioConfig);

        WHEN ("the input bit is high")
        {
            Utils::SetBit(GPIOC->IDR, 5UL, true);

            THEN ("the input shall read true and the ouput shall stay false")
            {
                REQUIRE (gpio.GetInputState(gpioConfig.pin) == true);
                REQUIRE (gpio.GetOutputState(gpioConfig.pin) == false);
            }
            AND_WHEN ("the input bit is low")
            {
                Utils::SetBit(GPIOC->IDR, 5UL, false);

                THEN ("the input shall read false")
                {
                    REQUIRE (gpio.GetInputState(gpioConfig.pin) == false);
                }
            }
        }
    }

    GIVEN ("PC5 is configured as an output")
    {
        Hal::Gpio gpio = Hal::Gpio();
        Hal::gpioConfig_t gpioConfig =
        {
            .pin = {.port = Hal::Port::c, .number = 5UL},
            .mode = Hal::GpioMode::output,
            .isOpenDrain = false,
            .speed = Hal::GpioSpeed::low,
            .pull = Hal::GpioPull::pullUp,
            .alternateFunction = Hal::AlternateFunction::sys1
        };
        gpio.SetConfiguration(gpioConfig);

        WHEN ("the output bit is high")
        {
            Utils::SetBit(GPIOC->ODR, 5UL, true);

            THEN ("the output shall read true and the input shall stay false")
            {
                REQUIRE (gpio.GetOutputState(gpioConfig.pin) == true);
                REQUIRE (gpio.GetInputState(gpioConfig.pin) == false);
            }
            AND_WHEN ("the output bit is low")
            {
                Utils::SetBit(GPIOC->ODR, 5UL, false);

                THEN ("the output shall read false")
                {
                    REQUIRE (gpio.GetOutputState(gpioConfig.pin) == false);
                }
            }
        }
    }

    GIVEN ("PC5 is configured as an output")
    {
        Hal::Gpio gpio = Hal::Gpio();
        Hal::gpioConfig_t gpioConfig =
        {
            .pin = {.port = Hal::Port::c, .number = 5UL},
            .mode = Hal::GpioMode::output,
            .isOpenDrain = false,
            .speed = Hal::GpioSpeed::low,
            .pull = Hal::GpioPull::pullUp,
            .alternateFunction = Hal::AlternateFunction::sys1
        };
        gpio.SetConfiguration(gpioConfig);

        WHEN ("the output is set high")
        {
            gpio.SetOutputState(gpioConfig.pin, true);

            THEN ("the bit 5 in BSRR shall be set active")
            {
                REQUIRE (GPIOC->BSRR == Utils::Bit(5UL));
            }
            AND_WHEN ("the output is set low")
            {
                GPIOC->BSRR = 0UL; // Clear BSRR first to simulate HW behaviour.
                gpio.SetOutputState(gpioConfig.pin, false);

                THEN ("the bit 21 in BSRR shall be set active")
                {
                    REQUIRE (GPIOC->BSRR == Utils::Bit(21UL));
                }
            }
        }
    }
}

SCENARIO ("Developer uses GPIO wrong", "[gpio]")
{
    Hal_Mock::InitGpioRegisters();
    ASchMock::Assert::Init();

    GIVEN ("a GPIO class is created and GPIO configuration struct is created for PA16")
    {
        Hal::Gpio gpio = Hal::Gpio();
        Hal::gpioConfig_t gpioConfig =
        {
            .pin = {.port = Hal::Port::a, .number = 16UL}
        };

        WHEN ("trying to set output with the struct")
        {
            gpio.SetOutputState(gpioConfig.pin, true);

            THEN ("an assert failure shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
        AND_WHEN ("trying to get input with the struct")
        {
            (void)gpio.GetInputState(gpioConfig.pin);

            THEN ("an assert failure shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
        AND_WHEN ("trying to get output with the struct")
        {
            (void)gpio.GetOutputState(gpioConfig.pin);

            THEN ("an assert failure shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }
}
