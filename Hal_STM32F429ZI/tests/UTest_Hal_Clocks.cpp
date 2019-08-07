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

//! @file    UTest_Hal_Clocks.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    25 Jul 2019
//! 
//! @brief   These are unit tests for Hal_Clocks.cpp
//! 
//! These are unit tests for Hal_Clocks.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Catch_Utils.hpp>

#include <Hal_Clocks.hpp>
#include <Utils_Bit.hpp>
#include <stm32f429xx_mock.h>
#include <Utils_Assert_Mock.hpp>

#include <Hal_Internal_Mock.hpp>
#include <ASch_Configuration.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Clocks are enabled", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();
    HalMock::InitInternal();

    GIVEN ("HSI is disabled and clocks are functioning correctly")
    {
        // HSI is on by default, so disable it first.
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, false);

        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the HSI clock is enabled")
        {
            Hal::Clocks::Enable(Hal::OscillatorType::highSpeed_internal);

            THEN ("HSION bit in RCC CR register shall be set and HSIRDY bit shall be waited")
            {
                REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_HSION_Pos) == true);
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToSet, RCC->CR, RCC_CR_HSIRDY_Pos);
            }
        }
    }

    GIVEN ("HSE is disabled and clocks are functioning correctly")
    {
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the HSE clock is enabled")
        {
            Hal::Clocks::Enable(Hal::OscillatorType::highSpeed_external);

            THEN ("HSEON bit in RCC CR register shall be set and HSERDY bit shall be waited")
            {
                REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_HSEON_Pos) == true);
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToSet, RCC->CR, RCC_CR_HSERDY_Pos);
            }
        }
    }

    GIVEN ("LSI is disabled and clocks are functioning correctly")
    {
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the LSI clock is enabled")
        {
            Hal::Clocks::Enable(Hal::OscillatorType::lowSpeed_internal);

            THEN ("LSION bit in RCC CSR register shall be set and LSIRDY bit shall be waited")
            {
                REQUIRE (Utils::GetBit(RCC->CSR, RCC_CSR_LSION_Pos) == true);
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToSet, RCC->CSR, RCC_CSR_LSIRDY_Pos);
            }
        }
    }

    GIVEN ("LSE is disabled and clocks are functioning correctly")
    {
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the LSE clock is enabled")
        {
            Hal::Clocks::Enable(Hal::OscillatorType::lowSpeed_external);

            THEN ("LSEON bit in RCC BDCR register shall be set and LSERDY bit shall be waited")
            {
                REQUIRE (Utils::GetBit(RCC->BDCR, RCC_BDCR_LSEON_Pos) == true);
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToSet, RCC->BDCR, RCC_BDCR_LSERDY_Pos);
            }
        }
    }

    GIVEN ("the oscillators are already running")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);
        Utils::SetBit(RCC->CSR, RCC_CSR_LSIRDY_Pos, true);
        Utils::SetBit(RCC->BDCR, RCC_BDCR_LSERDY_Pos, true);

        WHEN ("the the clocks are enabled")
        {
            Hal::Clocks::Enable(Hal::OscillatorType::highSpeed_internal);
            Hal::Clocks::Enable(Hal::OscillatorType::highSpeed_external);
            Hal::Clocks::Enable(Hal::OscillatorType::lowSpeed_internal);
            Hal::Clocks::Enable(Hal::OscillatorType::lowSpeed_external);

            THEN ("no configurations shall occur")
            {
                REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitToSet);
            }
        }
    }
}

SCENARIO ("Clock status is read", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();

    GIVEN ("HSI is enabled")
    {
        // HSI is on by default

        WHEN ("the HSI status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::highSpeed_internal);

            THEN ("the check shall return true")
            {
                REQUIRE (isRunning == true);
            }
        }
    }

    GIVEN ("HSI is disabled")
    {
        // Disable HSI
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, false);

        WHEN ("the HSI status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::highSpeed_internal);

            THEN ("the check shall return false")
            {
                REQUIRE (isRunning == false);
            }
        }
    }

    GIVEN ("HSE is enabled")
    {
        // Enable HSE
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);

        WHEN ("the HSE status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::highSpeed_external);

            THEN ("the check shall return true")
            {
                REQUIRE (isRunning == true);
            }
        }
    }

    GIVEN ("HSE is disabled")
    {
        // HSE is disabled by default

        WHEN ("the HSE status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::highSpeed_external);

            THEN ("the check shall return false")
            {
                REQUIRE (isRunning == false);
            }
        }
    }

    GIVEN ("LSI is enabled")
    {
        // Enable LSI
        Utils::SetBit(RCC->CSR, RCC_CSR_LSIRDY_Pos, true);

        WHEN ("the LSI status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::lowSpeed_internal);

            THEN ("the check shall return true")
            {
                REQUIRE (isRunning == true);
            }
        }
    }

    GIVEN ("LSI is disabled")
    {
        // LSI is disabled by default

        WHEN ("the LSI status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::lowSpeed_internal);

            THEN ("the check shall return false")
            {
                REQUIRE (isRunning == false);
            }
        }
    }

    GIVEN ("LSE is enabled")
    {
        // Enable LSE
        Utils::SetBit(RCC->BDCR, RCC_BDCR_LSERDY_Pos, true);

        WHEN ("the LSE status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::lowSpeed_external);

            THEN ("the check shall return true")
            {
                REQUIRE (isRunning == true);
            }
        }
    }

    GIVEN ("LSE is disabled")
    {
        // LSE is disabled by default

        WHEN ("the LSE status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::lowSpeed_external);

            THEN ("the check shall return false")
            {
                REQUIRE (isRunning == false);
            }
        }
    }

    GIVEN ("PLL is enabled")
    {
        // Enable PLL
        Utils::SetBit(RCC->CR, RCC_CR_PLLRDY_Pos, true);

        WHEN ("the PLL status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::pll);

            THEN ("the check shall return true")
            {
                REQUIRE (isRunning == true);
            }
        }
    }

    GIVEN ("PLL is disabled")
    {
        // PLL is disabled by default

        WHEN ("the PLL status is checked")
        {
            bool isRunning = Hal::Clocks::IsRunning(Hal::OscillatorType::pll);

            THEN ("the check shall return false")
            {
                REQUIRE (isRunning == false);
            }
        }
    }
}

SCENARIO ("Clocks are disabled", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();
    HalMock::InitInternal();

    GIVEN ("HSI is enabled and is not used as a system clock")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, true);
        // HSE as system clock.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_0);

        // This tells the HAL that the oscillator has stopped correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the HSI clock is disabled")
        {
            Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_internal);

            THEN ("HSION bit in RCC CR register shall be cleared and HSERDY bit clearing shall be waited")
            {
                REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_HSION_Pos) == false);
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToClear, RCC->CR, RCC_CR_HSIRDY_Pos);
            }
        }
    }

    GIVEN ("HSE is enabled and clocks are functioning correctly")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSEON_Pos, true);
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);

        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the HSE clock is disabled")
        {
            Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_external);

            THEN ("HSEON bit in RCC CR register shall be cleared and HSERDY bit clearing shall be waited")
            {
                REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_HSEON_Pos) == false);
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToClear, RCC->CR, RCC_CR_HSERDY_Pos);
            }
        }
    }

    GIVEN ("LSI is enabled and clocks are functioning correctly")
    {
        Utils::SetBit(RCC->CSR, RCC_CSR_LSION_Pos, true);
        Utils::SetBit(RCC->CSR, RCC_CSR_LSIRDY_Pos, true);

        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the LSI clock is disabled")
        {
            Hal::Clocks::Disable(Hal::OscillatorType::lowSpeed_internal);

            THEN ("LSION bit in RCC CSR register shall be cleared and LSIRDY bit clearing shall be waited")
            {
                REQUIRE (Utils::GetBit(RCC->CSR, RCC_CSR_LSION_Pos) == false);
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToClear, RCC->CSR, RCC_CSR_LSIRDY_Pos);
            }
        }
    }

    GIVEN ("LSE is enabled and clocks are functioning correctly")
    {
        Utils::SetBit(RCC->BDCR, RCC_BDCR_LSEON_Pos, true);
        Utils::SetBit(RCC->BDCR, RCC_BDCR_LSERDY_Pos, true);

        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the LSE clock is disabled")
        {
            Hal::Clocks::Disable(Hal::OscillatorType::lowSpeed_external);

            THEN ("LSEON bit in RCC BDCR register shall be cleared and LSERDY bit clearing shall be waited")
            {
                REQUIRE (Utils::GetBit(RCC->BDCR, RCC_BDCR_LSEON_Pos) == false);
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToClear, RCC->BDCR, RCC_BDCR_LSERDY_Pos);
            }
        }
    }

    GIVEN ("the oscillators are already disabled")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, false);

        WHEN ("the the clocks are enabled")
        {
            Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_internal);
            Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_external);
            Hal::Clocks::Disable(Hal::OscillatorType::lowSpeed_internal);
            Hal::Clocks::Disable(Hal::OscillatorType::lowSpeed_external);

            THEN ("no configurations shall occur")
            {
                REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitToClear);
            }
        }
    }

    GIVEN ("HSE is enabled and used as system clock")
    {
        // HSE is enabled
        Utils::SetBit(RCC->CR, RCC_CR_HSEON_Pos, true);
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);
        // Set HSE as system clock
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_0);

        WHEN ("the HSE clock is disabled")
        {
            Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_external);

            THEN ("no configurations shall occur")
            {
                REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitToClear);
            }
        }
    }
}

SCENARIO ("System clock is selected", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();
    HalMock::InitInternal();

    GIVEN ("HSE is system clock and HSI is enabled")
    {
        // Set HSE as system clock
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_0);
        // HSI is enabled by default. No need to enable it again.
        // Set invalid value to SW bits for checking that the bits are set afterwards.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk);
        // This tells the HAL that the system clock was changed successfully.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitPatternToSet, false);

        WHEN ("the HSI is set as system clock source")
        {
            Hal::Clocks::SetSysClockSource(Hal::OscillatorType::highSpeed_internal);

            THEN ("SWS bits in RCC CFGR register shall be set to 00 and corresponding SW bits shall be waited")
            {
                REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, 0UL));
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitPatternToSet, RCC->CFGR, (uint32_t)RCC_CFGR_SWS_Msk, (uint32_t)0UL);
            }
        }
    }

    GIVEN ("HSE is enabled")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);

        // Set invalid value to SW bits for checking that the bits are set afterwards.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk);
        // This tells the HAL that the system clock was changed successfully.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitPatternToSet, false);

        WHEN ("the HSI is set as system clock source")
        {
            Hal::Clocks::SetSysClockSource(Hal::OscillatorType::highSpeed_external);

            THEN ("SWS bits in RCC CFGR register shall be set to 01 and corresponding SW bits shall be waited")
            {
                REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_0));
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitPatternToSet, RCC->CFGR, (uint32_t)RCC_CFGR_SWS_Msk, (uint32_t)RCC_CFGR_SWS_0);
            }
        }
    }

    GIVEN ("PLL is enabled")
    {
        Utils::SetBit(RCC->CR, RCC_CR_PLLRDY_Pos, true);

        // Set invalid value to SW bits for checking that the bits are set afterwards.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk);
        // This tells the HAL that the system clock was changed successfully.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitPatternToSet, false);

        WHEN ("the PLL is set as system clock source")
        {
            Hal::Clocks::SetSysClockSource(Hal::OscillatorType::pll);

            THEN ("SWS bits in RCC CFGR register shall be set to 10 and corresponding SW bits shall be waited")
            {
                REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_1));
                REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitPatternToSet, RCC->CFGR, (uint32_t)RCC_CFGR_SWS_Msk, (uint32_t)RCC_CFGR_SWS_1);
            }
        }
    }

    GIVEN ("HSE is system clock and HSI is disabled")
    {
        // Set HSE as system clock
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_0);
        // Disable HSI.
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, false);
        
        // Set invalid value to SW bits for checking that the bits are set afterwards.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk);
        // This tells the HAL that the system clock was changed successfully.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitPatternToSet, false);

        WHEN ("the HSI is set as system clock source")
        {
            Hal::Clocks::SetSysClockSource(Hal::OscillatorType::highSpeed_internal);

            THEN ("SWS bits in RCC CFGR register shall not be changed")
            {
                REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk));
                REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitPatternToSet);
            }
        }
    }

    GIVEN ("HSE is disabled")
    {
        // HSE is disabled by default.
        // Set invalid value to SW bits for checking that the bits are set afterwards.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk);
        // This tells the HAL that the system clock was changed successfully.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitPatternToSet, false);

        WHEN ("the HSI is set as system clock source")
        {
            Hal::Clocks::SetSysClockSource(Hal::OscillatorType::highSpeed_external);

            THEN ("SWS bits in RCC CFGR register shall note be changed")
            {
                REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk));
                REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitPatternToSet);
            }
        }
    }

    GIVEN ("PLL is disabled")
    {
        // PLL is disabled by default.
        // Set invalid value to SW bits for checking that the bits are set afterwards.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk);
        // This tells the HAL that the system clock was changed successfully.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitPatternToSet, false);

        WHEN ("the PLL is set as system clock source")
        {
            Hal::Clocks::SetSysClockSource(Hal::OscillatorType::pll);

            THEN ("SWS bits in RCC CFGR register shall note be changed")
            {
                REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk));
                REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitPatternToSet);
            }
        }
    }
}

SCENARIO ("System clock source is read", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();

    GIVEN ("HSI is system clock")
    {
        // HSI is system clock by default

        WHEN ("system clock source is read")
        {
            Hal::OscillatorType type = Hal::Clocks::GetSysClockSource();

            THEN ("the clock type shall be HSI")
            {
                REQUIRE (type == Hal::OscillatorType::highSpeed_internal);
            }
        }
    }

    GIVEN ("HSE is system clock")
    {
        // Set HSE as system clock
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_0);

        WHEN ("system clock source is read")
        {
            Hal::OscillatorType type = Hal::Clocks::GetSysClockSource();

            THEN ("the clock type shall be HSE")
            {
                REQUIRE (type == Hal::OscillatorType::highSpeed_external);
            }
        }
    }

    GIVEN ("PLL is system clock")
    {
        // Set PLL as system clock
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_1);

        WHEN ("system clock source is read")
        {
            Hal::OscillatorType type = Hal::Clocks::GetSysClockSource();

            THEN ("the clock type shall be PLL")
            {
                REQUIRE (type == Hal::OscillatorType::pll);
            }
        }
    }
}
