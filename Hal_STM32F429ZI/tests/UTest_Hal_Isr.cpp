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

//! @file    UTest_Hal_Isr.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    29 Aug 2018
//! 
//! @brief   These are unit tests for Hal_Isr.cpp
//! 
//! These are unit tests for Hal_Isr.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Catch_Utils.hpp>

#include <Hal_Isr.hpp>
#include <Utils_Bit.hpp>
#include <stm32f429xx_mock.h>
#include <Utils_Assert_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

void TestIsrHandler(void)
{
    return;
}

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Interrupt module is initialised", "[isr]")
{
    ASchMock::Assert::Init();
    
    GIVEN ("an ISR instance is created")
    {
        Hal::Isr isr = Hal::Isr();

        WHEN ("the ISR module is initialised")
        {
            isr.Init();

            THEN ("VTOR offset register shall be set to non-zero")
            {
                REQUIRE (SCB->VTOR != 0UL);
            }
        }
    }
}

SCENARIO ("Interrupt handler is set", "[isr]")
{
    ASchMock::Assert::Init();
    
    GIVEN ("an ISR module is initialised")
    {
        Hal::Isr isr = Hal::Isr();
        isr.Init();

        WHEN ("a handler for SysTick is set")
        {
            isr.SetHandler(Hal::Interrupt::sysTick, TestIsrHandler);

            THEN ("fetched SysTick handler shall match with the test handler")
            {
                REQUIRE (isr.GetHandler(Hal::Interrupt::sysTick) == static_cast<Hal::interruptHandler_t>(TestIsrHandler));
            }
            AND_THEN ("the handler shall also be visible via VTOR")
            {
                Hal::interruptHandler_t* pVectors = reinterpret_cast<Hal::interruptHandler_t*>(SCB->VTOR);
                REQUIRE (pVectors[NVIC_USER_IRQ_OFFSET + static_cast<int32_t>(SysTick_IRQn)] == static_cast<Hal::interruptHandler_t>(TestIsrHandler));
            }
        }
    }

    GIVEN ("an ISR module is initialised")
    {
        Hal::Isr isr = Hal::Isr();
        isr.Init();

        WHEN ("a null handler for ADC is set")
        {
            isr.SetHandler(Hal::Interrupt::adc, 0);

            THEN ("an assert failure shall occur due to invalid handler")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }
}

SCENARIO ("Interrupt enabled/disabled", "[isr]")
{
    Hal_Mock::InitNvicRegisters();

    GIVEN ("an ISR module is initialised and handler for Timer 12 is set")
    {
        Hal::Isr isr = Hal::Isr();
        isr.Init();
        isr.SetHandler(Hal::Interrupt::tim12, TestIsrHandler);

        WHEN ("timer 12 interrupt is enabled")
        {
            isr.Enable(Hal::Interrupt::tim12);

            THEN ("timer 12 interrupt enable bit shall be set in NVIC ISER register")
            {
                REQUIRE (NVIC_GetEnableIRQ(TIM8_BRK_TIM12_IRQn) != 0UL);
            }
            AND_THEN ("timer 12 interrupt disable bit shall not be set in NVIC ICER register")
            {
                REQUIRE (NVIC_GetDisableIRQ(TIM8_BRK_TIM12_IRQn) == 0UL);
            }
            AND_WHEN ("timer 12 interrupt is disabled")
            {
                isr.Disable(Hal::Interrupt::tim12);

                THEN ("timer 12 interrupt disable bit shall be set in NVIC ICER register")
                {
                    REQUIRE (NVIC_GetDisableIRQ(TIM8_BRK_TIM12_IRQn) != 0UL);
                }
            }
        }
    }
}

SCENARIO ("Interrupt enabled without a handler", "[isr]")
{
    ASchMock::Assert::Init();

    GIVEN ("an ISR module is initialised")
    {
        Hal::Isr isr = Hal::Isr();
        isr.Init();

        WHEN ("SPI3 interrupt is enabled")
        {
            isr.Enable(Hal::Interrupt::spi3);

            THEN ("an assert failure shall occur since SPI3 does not have an ISR handler configured")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }
}

SCENARIO ("Interrupt priority is configured", "[isr]")
{
    Hal_Mock::InitNvicRegisters();
    ASchMock::Assert::Init();

    GIVEN ("an ISR module is initialised")
    {
        Hal::Isr isr = Hal::Isr();
        isr.Init();

        WHEN ("USART1 interrupt priority is set to 10")
        {
            isr.SetPriority(Hal::Interrupt::usart1, 10UL);

            THEN ("USART1 interrupt priority in NVIC registers shall be set to the same value")
            {
                REQUIRE (NVIC_GetPriority(USART1_IRQn) == 10UL);
            }
            AND_THEN ("the priority will be available from the API too")
            {
                REQUIRE (isr.GetPriority(Hal::Interrupt::usart1) == 10UL);
            }
        }
    }

    GIVEN ("an ISR module is initialised")
    {
        Hal::Isr isr = Hal::Isr();
        isr.Init();

        WHEN ("NMI interrupt priority is set to any value")
        {
            isr.SetPriority(Hal::Interrupt::nonMaskableInt, 0UL);

            THEN ("an assert failure shall occur since NMI has fixed priority")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }
}

SCENARIO ("Interrupt is set pending and pending state is read", "[isr]")
{
    Hal_Mock::InitNvicRegisters();

    GIVEN ("an ISR module is initialised and handler for EXTI0 is set")
    {
        Hal::Isr isr = Hal::Isr();
        isr.Init();
        isr.SetHandler(Hal::Interrupt::exti0, TestIsrHandler);

        WHEN ("EXTI0 interrupt is not pending")
        {
            THEN ("EXTI0 shall not be reported to be pending")
            {
                REQUIRE (isr.GetPending(Hal::Interrupt::exti0) == false);
            }
            AND_WHEN ("EXTI0 interrupt is set pending")
            {
                isr.SetPending(Hal::Interrupt::exti0);

                THEN ("EXTI0 interrupt is set pending in NVIC ISPR register")
                {
                    REQUIRE (NVIC_GetPendingIRQ(EXTI0_IRQn) != 0UL);
                }
                AND_THEN ("the priority pending information will be available from the API too")
                {
                    REQUIRE (isr.GetPending(Hal::Interrupt::exti0) == true);
                }
            }
        }
    }
}

SCENARIO ("Interrupt is cleared", "[isr]")
{
    Hal_Mock::InitNvicRegisters();

    GIVEN ("an ISR module is initialised, handler for RNG is set and RNG interrupt is set pending")
    {
        Hal::Isr isr = Hal::Isr();
        isr.Init();
        isr.SetHandler(Hal::Interrupt::hashAndRng, TestIsrHandler);
        isr.SetPending(Hal::Interrupt::hashAndRng);

        WHEN ("RNG interrupt is cleared")
        {
            isr.Clear(Hal::Interrupt::hashAndRng);

            THEN ("RNG interrupt pending clear bit is set in NVIC ICPR register")
            {
                REQUIRE (NVIC_GetClearPendingIRQ(HASH_RNG_IRQn) != 0UL);
            }
        }
    }
}
