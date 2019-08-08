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

#define PRINT_PLL()     uint32_t pllp = (RCC->PLLCFGR & RCC_PLLCFGR_PLLP_Msk) >> RCC_PLLCFGR_PLLP_Pos; \
                        uint32_t plln = (RCC->PLLCFGR & RCC_PLLCFGR_PLLN_Msk) >> RCC_PLLCFGR_PLLN_Pos; \
                        uint32_t pllm = (RCC->PLLCFGR & RCC_PLLCFGR_PLLM_Msk) >> RCC_PLLCFGR_PLLM_Pos; \
                        uint32_t pllq = (RCC->PLLCFGR & RCC_PLLCFGR_PLLQ_Msk) >> RCC_PLLCFGR_PLLQ_Pos; \
                        INFO ("PLLCFGR: " << std::hex << RCC->PLLCFGR); \
                        INFO ("PLLP: " << pllp); \
                        INFO ("PLLN: " << plln); \
                        INFO ("PLLM: " << pllm); \
                        INFO ("PLLQ: " << pllq);

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Clocks are enabled", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();
    HalMock::InitInternal();
    ASchMock::Assert::Init();

    GIVEN ("HSI is disabled and clocks are functioning correctly")
    {
        // HSI is on by default, so disable it first.
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, false);

        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the HSI clock is enabled")
        {
            Hal::Error error = Hal::Clocks::Enable(Hal::OscillatorType::highSpeed_internal);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);
                
                AND_THEN ("HSION bit in RCC CR register shall be set and HSIRDY bit shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_HSION_Pos) == true);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToSet, RCC->CR, RCC_CR_HSIRDY_Pos);
                }
            }
        }
    }

    GIVEN ("HSE is disabled and clocks are functioning correctly")
    {
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the HSE clock is enabled")
        {
            Hal::Error error = Hal::Clocks::Enable(Hal::OscillatorType::highSpeed_external);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);
                
                AND_THEN ("HSEON bit in RCC CR register shall be set and HSERDY bit shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_HSEON_Pos) == true);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToSet, RCC->CR, RCC_CR_HSERDY_Pos);
                }
            }
        }
    }

    GIVEN ("LSI is disabled and clocks are functioning correctly")
    {
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the LSI clock is enabled")
        {
            Hal::Error error = Hal::Clocks::Enable(Hal::OscillatorType::lowSpeed_internal);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("LSION bit in RCC CSR register shall be set and LSIRDY bit shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->CSR, RCC_CSR_LSION_Pos) == true);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToSet, RCC->CSR, RCC_CSR_LSIRDY_Pos);
                }
            }
        }
    }

    GIVEN ("LSE is disabled and clocks are functioning correctly")
    {
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the LSE clock is enabled")
        {
            Hal::Error error = Hal::Clocks::Enable(Hal::OscillatorType::lowSpeed_external);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                THEN ("LSEON bit in RCC BDCR register shall be set and LSERDY bit shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->BDCR, RCC_BDCR_LSEON_Pos) == true);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToSet, RCC->BDCR, RCC_BDCR_LSERDY_Pos);
                }
            }
        }
    }

    GIVEN ("PLL is disabled and clocks are functioning correctly")
    {
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the PLL clock is enabled")
        {
            Hal::Error error = Hal::Clocks::Enable(Hal::OscillatorType::pll);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                THEN ("LSEON bit in RCC BDCR register shall be set and LSERDY bit shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_PLLON_Pos) == true);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToSet, RCC->CR, RCC_CR_PLLRDY_Pos);
                }
            }
        }
    }

    GIVEN ("the oscillators are already running")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);
        Utils::SetBit(RCC->CSR, RCC_CSR_LSIRDY_Pos, true);
        Utils::SetBit(RCC->BDCR, RCC_BDCR_LSERDY_Pos, true);

        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("the the clocks are enabled")
        {
            Hal::Error error = Hal::Clocks::Enable(Hal::OscillatorType::highSpeed_internal);
            Hal::Error error2 = Hal::Clocks::Enable(Hal::OscillatorType::highSpeed_external);
            Hal::Error error3 = Hal::Clocks::Enable(Hal::OscillatorType::lowSpeed_internal);
            Hal::Error error4 = Hal::Clocks::Enable(Hal::OscillatorType::lowSpeed_external);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);
                REQUIRE (error2 == Hal::Error::noErrors);
                REQUIRE (error3 == Hal::Error::noErrors);
                REQUIRE (error4 == Hal::Error::noErrors);

                AND_THEN ("no configurations shall occur")
                {
                    REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitToSet);
                }
            }
        }
    }

    GIVEN ("HSE is disabled and clock fails to start")
    {
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, true);

        WHEN ("the HSE clock is enabled")
        {
            Hal::Error error = Hal::Clocks::Enable(Hal::OscillatorType::highSpeed_external);

            THEN ("timeout error shall occur")
            {
                REQUIRE (error == Hal::Error::timeout);
            }
        }
    }

    GIVEN ("the system is in init state")
    {
        WHEN ("secondary PLL is enabled directly")
        {
            Hal::Error error = Hal::Clocks::Enable(Hal::OscillatorType::pllSecondary);

            THEN ("parameter error shall occur")
            {
                REQUIRE (error == Hal::Error::invalidParameter);

                AND_THEN ("an assert failure shall trigger")
                {
                    REQUIRE (ASchMock::Assert::GetFails() == 1UL);
                }
            }
        }
    }
}

SCENARIO ("Clocks are disabled", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();
    HalMock::InitInternal();
    ASchMock::Assert::Init();

    GIVEN ("HSI is enabled and is not used as a system clock")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSION_Pos, true);
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, true);
        // HSE as system clock.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_0);

        // This tells the HAL that the oscillator has stopped correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the HSI clock is disabled")
        {
            Hal::Error error = Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_internal);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("HSION bit in RCC CR register shall be cleared and HSERDY bit clearing shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_HSION_Pos) == false);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToClear, RCC->CR, RCC_CR_HSIRDY_Pos);
                }
            }
        }
    }

    GIVEN ("HSE is enabled and clocks are functioning correctly")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSEON_Pos, true);
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);

        // This tells the HAL that the oscillator has stopped correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the HSE clock is disabled")
        {
            Hal::Error error = Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_external);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("HSEON bit in RCC CR register shall be cleared and HSERDY bit clearing shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_HSEON_Pos) == false);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToClear, RCC->CR, RCC_CR_HSERDY_Pos);
                }
            }
        }
    }

    GIVEN ("LSI is enabled and clocks are functioning correctly")
    {
        Utils::SetBit(RCC->CSR, RCC_CSR_LSION_Pos, true);
        Utils::SetBit(RCC->CSR, RCC_CSR_LSIRDY_Pos, true);

        // This tells the HAL that the oscillator has stopped correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the LSI clock is disabled")
        {
            Hal::Error error = Hal::Clocks::Disable(Hal::OscillatorType::lowSpeed_internal);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("LSION bit in RCC CSR register shall be cleared and LSIRDY bit clearing shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->CSR, RCC_CSR_LSION_Pos) == false);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToClear, RCC->CSR, RCC_CSR_LSIRDY_Pos);
                }
            }
        }
    }

    GIVEN ("LSE is enabled and clocks are functioning correctly")
    {
        Utils::SetBit(RCC->BDCR, RCC_BDCR_LSEON_Pos, true);
        Utils::SetBit(RCC->BDCR, RCC_BDCR_LSERDY_Pos, true);

        // This tells the HAL that the oscillator has stopped correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the LSE clock is disabled")
        {
            Hal::Error error = Hal::Clocks::Disable(Hal::OscillatorType::lowSpeed_external);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("LSEON bit in RCC BDCR register shall be cleared and LSERDY bit clearing shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->BDCR, RCC_BDCR_LSEON_Pos) == false);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToClear, RCC->BDCR, RCC_BDCR_LSERDY_Pos);
                }
            }
        }
    }

    GIVEN ("PLL is enabled and clocks are functioning correctly")
    {
        Utils::SetBit(RCC->CR, RCC_CR_PLLON_Pos, true);
        Utils::SetBit(RCC->CR, RCC_CR_PLLRDY_Pos, true);

        // This tells the HAL that the oscillator has stopped correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the PLL clock is disabled")
        {
            Hal::Error error = Hal::Clocks::Disable(Hal::OscillatorType::pll);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("PLLON bit in RCC CR register shall be cleared and PLLRDY bit clearing shall be waited")
                {
                    REQUIRE (Utils::GetBit(RCC->CR, RCC_CR_PLLON_Pos) == false);
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitToClear, RCC->CR, RCC_CR_PLLRDY_Pos);
                }
            }
        }
    }

    GIVEN ("the oscillators are already disabled")
    {
        // Disable HSI
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, false);
        // Disable HSI as system clock
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_Msk);

        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, false);

        WHEN ("the the clocks are enabled")
        {
            Hal::Error error = Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_internal);
            Hal::Error error2 = Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_external);
            Hal::Error error3 = Hal::Clocks::Disable(Hal::OscillatorType::lowSpeed_internal);
            Hal::Error error4 = Hal::Clocks::Disable(Hal::OscillatorType::lowSpeed_external);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);
                REQUIRE (error2 == Hal::Error::noErrors);
                REQUIRE (error3 == Hal::Error::noErrors);
                REQUIRE (error4 == Hal::Error::noErrors);

                AND_THEN ("no configurations shall occur")
                {
                    REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitToClear);
                }
            }
        }
    }

    GIVEN ("HSE is enabled and clock fails to stop")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSEON_Pos, true);
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);

        // This tells the HAL that the oscillator failed to stop.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToClear, true);

        WHEN ("the HSE clock is disabled")
        {
            Hal::Error error = Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_external);

            THEN ("timeout error shall occur")
            {
                REQUIRE (error == Hal::Error::timeout);
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
            Hal::Error error = Hal::Clocks::Disable(Hal::OscillatorType::highSpeed_external);

            THEN ("reserved resource error shall occur")
            {
                REQUIRE (error == Hal::Error::reservedResource);

                AND_THEN ("an assert failure shall trigger")
                {
                    REQUIRE (ASchMock::Assert::GetFails() == 1UL);
                }
            }
        }
    }

    GIVEN ("PLL is enabled")
    {
        // PLL is enabled
        Utils::SetBit(RCC->CR, RCC_CR_PLLON_Pos, true);
        Utils::SetBit(RCC->CR, RCC_CR_PLLRDY_Pos, true);

        WHEN ("the secondary PLL clock is disabled")
        {
            Hal::Error error = Hal::Clocks::Disable(Hal::OscillatorType::pllSecondary);

            THEN ("invalid parameter error shall occur")
            {
                REQUIRE (error == Hal::Error::invalidParameter);

                AND_THEN ("an assert failure shall trigger")
                {
                    REQUIRE (ASchMock::Assert::GetFails() == 1UL);
                }
            }
        }
    }
}

SCENARIO ("PLL is configured", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();
    HalMock::InitInternal();
    ASchMock::Assert::Init();

    GIVEN ("HSE is enabled and set as 24MHz")
    {
        // Enable HSE
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);
        // Set frequency
        Hal::Clocks::SetFrequency(Hal::OscillatorType::highSpeed_external, Hal::MHz(24UL));
        
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("PLL is enabled with source from HSE and target of 168MHz and the frequency is read back")
        {
            Hal::Error error = Hal::Clocks::ConfigurePll(Hal::OscillatorType::highSpeed_external, Hal::MHz(168UL));
            Utils::SetBit(RCC->CR, RCC_CR_PLLRDY_Pos, true); // Set PLLRDY bit in order to be able to read the frequency.
            uint32_t frequency = Hal::Clocks::GetFrequency(Hal::OscillatorType::pll);
            uint32_t secondaryFrequency = Hal::Clocks::GetFrequency(Hal::OscillatorType::pllSecondary);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("PLLP, PLLN, PLLM, and PLLQ shall have correct values")
                {
                    // 100MHz from 24MHz shall result in PLLP = 2, PLLN = 100, PLLM = 11.
                    PRINT_PLL();
                    INFO ("Frequency: " << frequency);
                    INFO ("Frequency: " << secondaryFrequency);

                    REQUIRE (Utils::CompareBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk, (2UL << RCC_PLLCFGR_PLLP_Pos)));
                    REQUIRE (Utils::CompareBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, (56UL << RCC_PLLCFGR_PLLN_Pos)));
                    REQUIRE (Utils::CompareBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_Msk, (4UL << RCC_PLLCFGR_PLLM_Pos)));
                    REQUIRE (Utils::CompareBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_Msk, (7UL << RCC_PLLCFGR_PLLQ_Pos)));
                    
                    AND_THEN ("the PLL frequency shall be set to 168MHz and secondary frequency to 48MHz")
                    {
                        REQUIRE (frequency == Hal::MHz(168UL));
                        REQUIRE (secondaryFrequency == Hal::MHz(48UL));
                    }
                }
            }
        }
    }

    GIVEN ("HSE is enabled and set as 24MHz")
    {
        // Enable HSE
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);
        // Set frequency
        Hal::Clocks::SetFrequency(Hal::OscillatorType::highSpeed_external, Hal::MHz(24UL));
        
        // This tells the HAL that the oscillator has started correctly.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitToSet, false);

        WHEN ("PLL is enabled with source from HSE and target of 109MHz and the frequency is read back")
        {
            Hal::Error error = Hal::Clocks::ConfigurePll(Hal::OscillatorType::highSpeed_external, Hal::MHz(109UL));
            Utils::SetBit(RCC->CR, RCC_CR_PLLRDY_Pos, true); // Set PLLRDY bit in order to be able to read the frequency.
            uint32_t frequency = Hal::Clocks::GetFrequency(Hal::OscillatorType::pll);
            uint32_t secondaryFrequency = Hal::Clocks::GetFrequency(Hal::OscillatorType::pllSecondary);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("PLLP, PLLN, PLLM, and PLLQ shall have correct values")
                {
                    // 100MHz from 24MHz shall result in PLLP = 2, PLLN = 100, PLLM = 11.
                    PRINT_PLL();
                    INFO ("Frequency: " << frequency);
                    INFO ("Frequency: " << secondaryFrequency);

                    REQUIRE (Utils::CompareBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk, (2UL << RCC_PLLCFGR_PLLP_Pos)));
                    REQUIRE (Utils::CompareBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, (100UL << RCC_PLLCFGR_PLLN_Pos)));
                    REQUIRE (Utils::CompareBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_Msk, (11UL << RCC_PLLCFGR_PLLM_Pos)));
                    REQUIRE (Utils::CompareBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_Msk, (5UL << RCC_PLLCFGR_PLLQ_Pos)));

                    AND_THEN ("the PLL frequency shall be set to 109 090 909 Hz and secondary frequency to 43 636 363 Hz")
                    {
                        REQUIRE (frequency == 109090909UL);
                        REQUIRE (secondaryFrequency == 43636363UL);
                    }
                }
            }
        }
    }

    GIVEN ("system is at init state")
    {
        WHEN ("PLL is enabled with source from LSI and target of 120MHz")
        {
            Hal::Error error = Hal::Clocks::ConfigurePll(Hal::OscillatorType::lowSpeed_internal, Hal::MHz(120UL));

            THEN ("invalid parameter error shall occur")
            {
                REQUIRE (error == Hal::Error::invalidParameter);

                AND_THEN ("an assert failure shall trigger")
                {
                    REQUIRE (ASchMock::Assert::GetFails() == 1UL);
                }
            }
        }
    }

    GIVEN ("system is at init state")
    {
        WHEN ("PLL is enabled with source from HSI and target of 169MHz")
        {
            Hal::Error error = Hal::Clocks::ConfigurePll(Hal::OscillatorType::highSpeed_internal, Hal::MHz(169UL));

            THEN ("invalid parameter error shall occur")
            {
                REQUIRE (error == Hal::Error::invalidParameter);

                AND_THEN ("an assert failure shall trigger")
                {
                    REQUIRE (ASchMock::Assert::GetFails() == 1UL);
                }
            }
        }
    }

    GIVEN ("HSE is not running")
    {
        WHEN ("PLL is enabled with source from HSE and target of 120MHz")
        {
            Hal::Error error = Hal::Clocks::ConfigurePll(Hal::OscillatorType::highSpeed_external, Hal::MHz(120UL));

            THEN ("unavailable resource error shall occur")
            {
                REQUIRE (error == Hal::Error::unavailableResource);

                AND_THEN ("an assert failure shall trigger")
                {
                    REQUIRE (ASchMock::Assert::GetFails() == 1UL);
                }
            }
        }
    }

    GIVEN ("HSE is running, but frequency is not configured")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);
        Hal::Clocks::SetFrequency(Hal::OscillatorType::highSpeed_external, 0UL);

        WHEN ("PLL is enabled with source from non-running HSE and target of 120MHz")
        {
            Hal::Error error = Hal::Clocks::ConfigurePll(Hal::OscillatorType::highSpeed_external, Hal::MHz(120UL));

            THEN ("invalid parameter error shall occur")
            {
                REQUIRE (error == Hal::Error::invalidParameter);

                AND_THEN ("an assert failure shall trigger")
                {
                    REQUIRE (ASchMock::Assert::GetFails() == 1UL);
                }
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

SCENARIO ("Clock frequencies are written and read", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();
    ASchMock::Assert::Init();

    GIVEN ("HSI is enabled")
    {
        // HSI is on by default

        WHEN ("HSI frequency is read")
        {
            uint32_t frequency = Hal::Clocks::GetFrequency(Hal::OscillatorType::highSpeed_internal);

            THEN ("frequency shall be 16MHz")
            {
                REQUIRE (frequency == Hal::MHz(16UL));
            }
        }
    }

    GIVEN ("HSI is enabled")
    {
        // HSI is on by default

        WHEN ("the HSI frequency is set to 10MHz")
        {
            Hal::Clocks::SetFrequency(Hal::OscillatorType::highSpeed_internal, Hal::MHz(10UL));
            uint32_t frequency = Hal::Clocks::GetFrequency(Hal::OscillatorType::highSpeed_internal);

            THEN ("an assert failure shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);

                AND_THEN ("the frequency shall still be 16MHz")
                {
                    REQUIRE (frequency == Hal::MHz(16UL));
                }
            }
        }
    }

    GIVEN ("HSE is enabled and its frequency is set to 24MHz")
    {
        // Enable HSE
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);
        Hal::Clocks::SetFrequency(Hal::OscillatorType::highSpeed_external, Hal::MHz(24UL));

        WHEN ("the HSE frequency is read")
        {
            uint32_t frequency = Hal::Clocks::GetFrequency(Hal::OscillatorType::highSpeed_external);

            THEN ("frequency shall be 24MHz")
            {
                REQUIRE (frequency == Hal::MHz(24UL));
            }
        }
    }

    GIVEN ("HSE is disabled and its frequency is set to 24MHz")
    {
        // HSE is disabled by default.
        Hal::Clocks::SetFrequency(Hal::OscillatorType::highSpeed_external, Hal::MHz(24UL));

        WHEN ("the HSE frequency is read")
        {
            uint32_t frequency = Hal::Clocks::GetFrequency(Hal::OscillatorType::highSpeed_external);

            THEN ("frequency shall be 0MHz")
            {
                REQUIRE (frequency == 0UL);
            }
        }
    }

    GIVEN ("PLL is enabled")
    {
        Utils::SetBit(RCC->CR, RCC_CR_PLLRDY_Pos, true);

        WHEN ("the PLL frequency is set to 100MHz")
        {
            Hal::Clocks::SetFrequency(Hal::OscillatorType::pll, Hal::MHz(100UL));

            THEN ("an assert failure shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }

    GIVEN ("PLL is enabled")
    {
        Utils::SetBit(RCC->CR, RCC_CR_PLLRDY_Pos, true);

        WHEN ("the PLL secondary frequency is set to 48MHz")
        {
            Hal::Clocks::SetFrequency(Hal::OscillatorType::pllSecondary, Hal::MHz(48UL));

            THEN ("an assert failure shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }

    GIVEN ("system is at init state")
    {
        WHEN ("the unknown frequency is read")
        {
            uint32_t frequency = Hal::Clocks::GetFrequency(Hal::OscillatorType::unknown);

            THEN ("frequency shall be 0MHz")
            {
                REQUIRE (frequency == 0UL);
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
            Hal::Error error = Hal::Clocks::SetSysClockSource(Hal::OscillatorType::highSpeed_internal);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("SWS bits in RCC CFGR register shall be set to 00 and corresponding SW bits shall be waited")
                {
                    REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, 0UL));
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitPatternToSet, RCC->CFGR, (uint32_t)RCC_CFGR_SWS_Msk, (uint32_t)0UL);
                }
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
            Hal::Error error = Hal::Clocks::SetSysClockSource(Hal::OscillatorType::highSpeed_external);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("SWS bits in RCC CFGR register shall be set to 01 and corresponding SW bits shall be waited")
                {
                    REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_0));
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitPatternToSet, RCC->CFGR, (uint32_t)RCC_CFGR_SWS_Msk, (uint32_t)RCC_CFGR_SWS_0);
                }
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
            Hal::Error error = Hal::Clocks::SetSysClockSource(Hal::OscillatorType::pll);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("SWS bits in RCC CFGR register shall be set to 10 and corresponding SW bits shall be waited")
                {
                    REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_1));
                    REQUIRE_PARAM_CALLS (1, HalMock::mockHalInternal, WaitForBitPatternToSet, RCC->CFGR, (uint32_t)RCC_CFGR_SWS_Msk, (uint32_t)RCC_CFGR_SWS_1);
                }
            }
        }
    }

    GIVEN ("system is at init state")
    {
        // Set invalid value to SW bits for checking that the bits are set afterwards.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk);
        // This tells the HAL that the system clock was changed successfully.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitPatternToSet, false);

        WHEN ("the HSI is set as system clock source")
        {
            Hal::Error error = Hal::Clocks::SetSysClockSource(Hal::OscillatorType::highSpeed_internal);

            THEN ("no errors shall occur")
            {
                REQUIRE (error == Hal::Error::noErrors);

                AND_THEN ("SWS bits in RCC CFGR register shall not be changed")
                {
                    REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk));
                    REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitPatternToSet);
                }
            }
        }
    }

    GIVEN ("system is at init state")
    {
        // Set invalid value to SW bits for checking that the bits are set afterwards.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk);

        WHEN ("the LSE is set as system clock source")
        {
            Hal::Error error = Hal::Clocks::SetSysClockSource(Hal::OscillatorType::lowSpeed_external);

            THEN ("invalid parameter error shall occur")
            {
                REQUIRE (error == Hal::Error::invalidParameter);

                AND_THEN ("SWS bits in RCC CFGR register shall not be changed")
                {
                    REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk));
                    REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitPatternToSet);
                }
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
            Hal::Error error = Hal::Clocks::SetSysClockSource(Hal::OscillatorType::pll);

            THEN ("unavailable resource error shall occur")
            {
                REQUIRE (error == Hal::Error::unavailableResource);

                AND_THEN ("SWS bits in RCC CFGR register shall note be changed")
                {
                    REQUIRE (Utils::CompareBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk));
                    REQUIRE_CALLS (0, HalMock::mockHalInternal, WaitForBitPatternToSet);
                }
            }
        }
    }

    GIVEN ("HSE is enabled")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSERDY_Pos, true);

        // Set invalid value to SW bits for checking that the bits are set afterwards.
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_Msk);
        // This tells the HAL that the system clock could not be changed.
        SET_RETURN(HalMock::mockHalInternal, WaitForBitPatternToSet, true);

        WHEN ("the HSI is set as system clock source")
        {
            Hal::Error error = Hal::Clocks::SetSysClockSource(Hal::OscillatorType::highSpeed_external);

            THEN ("timeout error shall occur")
            {
                REQUIRE (error == Hal::Error::timeout);
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
