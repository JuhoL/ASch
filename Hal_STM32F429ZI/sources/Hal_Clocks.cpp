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

typedef struct
{
    uint32_t n;
    uint32_t d;
} fraction_t;

// The default clock at boot is 16 MHz HSI DCO.
const uint32_t defaultSystemClock = 16000000UL;

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

/// @brief A function that converts a decimal into a fraction using Stern-Brocot tree.
/// @param decimal - The decimal to be converted.
/// @param fraction - A reference to the fraction struct for storing the result.
void FloatToFraction(float decimal, fraction_t& fraction);

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
uint32_t Clocks::clockFrequencies[static_cast<std::size_t>(OscillatorType::unknown)] =
{
    defaultSystemClock, // highSpeed_internal
    0UL,                // highSpeed_external
    0UL,                // lowSpeed_internal
    0UL,                // lowSpeed_external
    0UL                 // pll
};

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
    ASSERT(GetSysClockSource() != type);

    if (IsRunning(type) == true)
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

void Clocks::EnablePll(OscillatorType source, uint32_t frequency)
{
    ASSERT((source == OscillatorType::highSpeed_internal) || (source == OscillatorType::highSpeed_external));
    ASSERT(GetFrequency(source) > 0UL);
    ASSERT(IsRunning(source) == true);

    // The PLL frequency must be between 100MHz and 432MHz.
    ASSERT(frequency >= MHz(100UL));
    ASSERT(frequency <= MHz(432UL));

    // The PLL frequency is calculated by following formula:
    // fVCO = fsource * PLLN / PLLM
    // fPLL = fVCO / PLLP
    // fUSB = fVCO / PLLQ
    //
    // The values have following limitations:
    // PLLP = [2, 4, 6, 8]
    // PLLN >= 50, PLLN <= 432
    // PLLM >= 2, PLLN <= 63
    
    // The frequency formula can be converted into following form:
    // PLLN / (PLLM * PLLP) = fPLL / fsource
    // fPLL / fsource is calculated first.
    float frequencyRatio = static_cast<float>(frequency) / static_cast<float>(GetFrequency(source));

    // When we convert this decimal ratio into a fraction, we kno what PLLN and (PLLM * PLLP) are.
    fraction_t ratioFraction;
    FloatToFraction(frequencyRatio, ratioFraction);

    // Let's expand the fraction until it is within the value limits
    // The denominator must be even, so if the original denominator is odd we can skip odd expansion multipliers.
    uint32_t increment;
    if ((ratioFraction.d & 0x1UL) != 0UL)
    {
        increment = 2UL;
    }
    else
    {
        increment = 1UL;
    }

    // Minimum for nominator is minimum of PLLN, 50.
    const uint32_t nominatorMin = 50UL;
    // Minimum for denominator is (PLLMmin * PLLPmin) = 4
    const uint32_t denominatorMin = 4UL;

    uint32_t i = 0UL;
    fraction_t finalFraction = {.n = ratioFraction.n, .d = ratioFraction.d};
    while ((finalFraction.n < nominatorMin) || (finalFraction.d < denominatorMin))
    {
        i += increment;
        finalFraction.n = ratioFraction.n * i;
        finalFraction.d = ratioFraction.d * i;
    }

    // Set PLLN
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, (finalFraction.n << RCC_PLLCFGR_PLLN_Pos));

    // Let's select PLLP = 2. Set PLLP
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk, (2UL << RCC_PLLCFGR_PLLP_Pos));

    // Since PLLP = 2, PLLM is straightforward to calculate: Denominator / 2.
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_Msk, ((finalFraction.d >> 1UL) << RCC_PLLCFGR_PLLM_Pos));

    // Calculate the final frequency.
    clockFrequencies[static_cast<uint32_t>(OscillatorType::pll)] = static_cast<uint32_t>(static_cast<float>(finalFraction.n * GetFrequency(source)) 
                                                                                         / static_cast<float>(finalFraction.d));

    Utils::SetBit(RCC->CR, RCC_CR_PLLON_Pos, true);
    bool isTimeout = Internal::WaitForBitToSet(RCC->CR, RCC_CR_PLLRDY_Pos);
    (void)isTimeout;

    return;
}

bool Clocks::IsRunning(OscillatorType type)
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

void Clocks::SetFrequency(OscillatorType type, uint32_t frequency)
{
    ASSERT(type != OscillatorType::highSpeed_internal);
    ASSERT(type != OscillatorType::pll);
    ASSERT(type < OscillatorType::unknown);

    clockFrequencies[static_cast<uint32_t>(type)] = frequency;
    
    return;
}

uint32_t Clocks::GetFrequency(OscillatorType type)
{
    ASSERT_RETVAL(type < OscillatorType::unknown, 0UL);

    uint32_t frequency;
    if (IsRunning(type) == true)
    {
        frequency = clockFrequencies[static_cast<uint32_t>(type)];
    }
    else
    {
        frequency = 0UL;
    }
    return frequency;
}

uint32_t Clocks::GetSysClockFrequency(void)
{
    return sysClockFrequency;
}

void Clocks::SetSysClockSource(OscillatorType type)
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

OscillatorType Clocks::GetSysClockSource(void)
{
    OscillatorType type;
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

namespace
{

void FloatToFraction(float decimal, fraction_t& fraction)
{
    const float error = 0.01;
    uint32_t n = static_cast<uint32_t>(decimal);
    decimal -= static_cast<float>(n);
    
    if (decimal < error)
    {
        fraction.n = n;
        fraction.d = 1UL;
    }
    else if (1 - error < decimal)
    {
        fraction.n = n + 1UL;
        fraction.d = 1UL;
    }
    else
    {
        fraction_t lower = {.n = 0UL, .d = 1UL};
        fraction_t upper = {.n = 1UL, .d = 1UL};
        fraction_t middle;

        bool fractionFound = false;
        while (fractionFound == false)
        {
            middle.n = lower.n + upper.n;
            middle.d = lower.d + upper.d;
            
            if (static_cast<float>(middle.d) * (decimal + error) < static_cast<float>(middle.n))
            {
                upper.n = middle.n;
                upper.d = middle.d;
            }
            else if (static_cast<float>(middle.n) < (decimal - error) * static_cast<float>(middle.d))
            {
                lower.n = middle.n;
                lower.d = middle.d;
            }
            else
            {
                fraction.n = n * middle.d + middle.n;
                fraction.d = middle.d;
                fractionFound = true;
            }
        }
    }
    
    return;
}

}
