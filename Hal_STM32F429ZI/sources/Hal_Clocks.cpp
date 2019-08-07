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

//! @file    Hal_Clocks.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    25 Jul 2019
//!
//! @class   Clocks
//! @brief   !!!!! Brief file description here !!!!!
//! 
//! !!!!! Detailed file description here !!!!!

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Clocks.hpp>
#include <stm32f4xx.h>
#include <Utils_Bit.hpp>
#include <Utils_Assert.hpp>
#include <Hal_Internal.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

// The default clock at boot is 16 MHz HSI DCO.
const uint32_t defaultSystemClock = 16000000;

} // anonymous namespace

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Static Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{



} // anonymous namespace

//-----------------------------------------------------------------------------------------------------------------------------
// 6. Class Member Definitions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

//---------------------------------------
// Initialise static members
//---------------------------------------
uint32_t Clocks::sysClockFrequency = defaultSystemClock;

//---------------------------------------
// Functions
//---------------------------------------
void Clocks::Enable(OscillatorType type)
{
    if (IsRunning(type) == false)
    {
        bool isTimeout;

        switch (type)
        {
            case OscillatorType::highSpeed_internal:
                Utils::SetBit(RCC->CR, RCC_CR_HSION_Pos, true);
                isTimeout = Internal::WaitForBitToSet(RCC->CR, RCC_CR_HSIRDY_Pos);
                break;

            case OscillatorType::highSpeed_external:
                Utils::SetBit(RCC->CR, RCC_CR_HSEON_Pos, true);
                isTimeout = Internal::WaitForBitToSet(RCC->CR, RCC_CR_HSERDY_Pos);
                break;

            case OscillatorType::lowSpeed_internal:
                Utils::SetBit(RCC->CSR, RCC_CSR_LSION_Pos, true);
                isTimeout = Internal::WaitForBitToSet(RCC->CSR, RCC_CSR_LSIRDY_Pos);
                break;

            case OscillatorType::lowSpeed_external:
                Utils::SetBit(RCC->BDCR, RCC_BDCR_LSEON_Pos, true);
                isTimeout = Internal::WaitForBitToSet(RCC->BDCR, RCC_BDCR_LSERDY_Pos);
                break;

            default:
                // Do nothing.
                break;
        }

        (void)isTimeout;
    }
    return;
}

void Clocks::Disable(OscillatorType type)
{
    if ((IsRunning(type) == true) && (GetSysClockSource() != type))
    {
        bool isTimeout;

        switch (type)
        {
            case OscillatorType::highSpeed_internal:
                Utils::SetBit(RCC->CR, RCC_CR_HSION_Pos, false);
                isTimeout = Internal::WaitForBitToClear(RCC->CR, RCC_CR_HSIRDY_Pos);
                break;

            case OscillatorType::highSpeed_external:
                Utils::SetBit(RCC->CR, RCC_CR_HSEON_Pos, false);
                isTimeout = Internal::WaitForBitToClear(RCC->CR, RCC_CR_HSERDY_Pos);
                break;

            case OscillatorType::lowSpeed_internal:
                Utils::SetBit(RCC->CSR, RCC_CSR_LSION_Pos, false);
                isTimeout = Internal::WaitForBitToClear(RCC->CSR, RCC_CSR_LSIRDY_Pos);
                break;

            case OscillatorType::lowSpeed_external:
                Utils::SetBit(RCC->BDCR, RCC_BDCR_LSEON_Pos, false);
                isTimeout = Internal::WaitForBitToClear(RCC->BDCR, RCC_BDCR_LSERDY_Pos);
                break;

            default:
                // Do nothing.
                break;
        }

        (void)isTimeout;
    }
    return;
}

bool Clocks::IsRunning(Hal::OscillatorType type)
{
    bool isRunning;

    switch (type)
    {
        case OscillatorType::highSpeed_internal:
            isRunning = Utils::GetBit(RCC->CR, RCC_CR_HSIRDY_Pos);
            break;

        case OscillatorType::highSpeed_external:
            isRunning = Utils::GetBit(RCC->CR, RCC_CR_HSERDY_Pos);
            break;

        case OscillatorType::lowSpeed_internal:
            isRunning = Utils::GetBit(RCC->CSR, RCC_CSR_LSIRDY_Pos);
            break;

        case OscillatorType::lowSpeed_external:
            isRunning = Utils::GetBit(RCC->BDCR, RCC_BDCR_LSERDY_Pos);
            break;

        case OscillatorType::pll:
            isRunning = Utils::GetBit(RCC->CR, RCC_CR_PLLRDY_Pos);
            break;

        default:
            isRunning = false;
            break;
    }

    return isRunning;
}

uint32_t Clocks::GetSysClockFrequency(void)
{
    return sysClockFrequency;
}

void Clocks::SetSysClockSource(Hal::OscillatorType type)
{
    if (IsRunning(type) == true)
    {
        bool isTimeout;

        switch (type)
        {
            case OscillatorType::highSpeed_internal:
                Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, 0UL);
                isTimeout = Internal::WaitForBitPatternToSet(RCC->CFGR, RCC_CFGR_SWS_Msk, 0UL);
                break;

            case OscillatorType::highSpeed_external:
                Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_0);
                isTimeout = Internal::WaitForBitPatternToSet(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_0);
                break;

            case OscillatorType::pll:
                Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_1);
                isTimeout = Internal::WaitForBitPatternToSet(RCC->CFGR, RCC_CFGR_SWS_Msk, RCC_CFGR_SWS_1);
                break;

            default:
                // Do nothing.
                break;
        }

        (void)isTimeout;
    }
    return;
}

Hal::OscillatorType Clocks::GetSysClockSource(void)
{
    Hal::OscillatorType type;
    uint32_t sws = RCC->CFGR & RCC_CFGR_SWS_Msk;

    switch (sws)
    {
        case 0UL:
            type = OscillatorType::highSpeed_internal;
            break;

        case RCC_CFGR_SWS_0:
            type = OscillatorType::highSpeed_external;
            break;

        case RCC_CFGR_SWS_1:
            type = OscillatorType::pll;
            break;

        default:
            type = OscillatorType::unknown;
            break;
    }

    return type;
}

} // namespace Hal

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------
