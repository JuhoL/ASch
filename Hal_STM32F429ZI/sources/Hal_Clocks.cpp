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
//! @brief   This is a clocks HAL interface.
//! 
//! This class handles clock configurations.

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
const uint32_t defaultSystemClock = Hal::MHz(16UL);
// The LSE clock is tuned to 32.768kHz.
const uint32_t lseFrequency = 32768UL;
// The LSI clock is roughly 32kHz.
const uint32_t lsiFrequency = Hal::kHz(32UL);

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

/// @brief A function that converts a plain PLLP value into register format.
/// I.e. PLLP 2 -> 0, PLLP 8 -> 3
/// @param pllp - The PLLP divider value.
/// @return Returns the corresponding register value. Returns 4 if the value is invalid.
uint32_t ConvertPllp(uint32_t pllp);

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
    lsiFrequency,       // lowSpeed_internal
    lseFrequency,       // lowSpeed_external
    0UL,                // pll
    0UL                 // pllSecondary
};

//---------------------------------------
// Functions
//---------------------------------------
Error Clocks::Enable(OscillatorType type)
{
    ASSERT_RETVAL(type <= OscillatorType::pll, Error::invalidParameter);

    Error error = Error::noErrors;

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

            case OscillatorType::pll:
                Utils::SetBit(RCC->CR, RCC_CR_PLLON_Pos, true);
                isTimeout = Internal::WaitForBitToSet(RCC->CR, RCC_CR_PLLRDY_Pos);
                break;

            default:
                // This state should be already catched in sserts, but better to be safe.
                isTimeout = false;
                error = Error::invalidParameter;
                break;
        }

        if (isTimeout == true)
        {
            error = Error::timeout;
        }
    }

    return error;
}

Error Clocks::Disable(OscillatorType type)
{
    ASSERT_RETVAL(type <= OscillatorType::pll, Error::invalidParameter);
    ASSERT_RETVAL(GetSysClockSource() != type, Error::reservedResource);

    Error error = Error::noErrors;

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

            case OscillatorType::pll:
                Utils::SetBit(RCC->CR, RCC_CR_PLLON_Pos, false);
                isTimeout = Internal::WaitForBitToClear(RCC->CR, RCC_CR_PLLRDY_Pos);
                break;

            default:
                // This state should be already catched in sserts, but better to be safe.
                isTimeout = false;
                error = Error::invalidParameter;
                break;
        }

        if (isTimeout == true)
        {
            error = Error::timeout;
        }
    }

    return error;
}

Error Clocks::ConfigurePll(OscillatorType source, uint32_t frequency)
{
    ASSERT_RETVAL((source == OscillatorType::highSpeed_internal) || (source == OscillatorType::highSpeed_external), Error::invalidParameter);
    ASSERT_RETVAL(IsRunning(source) == true, Error::unavailableResource);
    ASSERT_RETVAL(GetFrequency(source) > 0UL, Error::invalidParameter);
    ASSERT_RETVAL(frequency <= MHz(168UL), Error::invalidParameter); // The PLL frequency maximum is 168MHz.

    Error error = Error::noErrors;

    // The PLL frequency is calculated by following formula:
    // fVCO = fsource * PLLN / PLLM
    // fPLL = fVCO / PLLP
    // fsecondary = fVCO / PLLQ
    //
    // The values have following limitations:
    // PLLP = [2, 4, 6, 8]
    // PLLN >= 50, PLLN <= 432
    // PLLM >= 2, PLLM <= 63
    // PLLQ >= 2, PLLQ <= 15
    
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
    // Maximum for nominator is minimum of PLLN, 432.
    const uint32_t nominatorMax = 432UL;
    // PLLM max is 63. If we choose PLLP = 2, maximum for denominator is (63 * 2)
    const uint32_t denominatorMax = 126UL;

    uint32_t i = 0UL;
    fraction_t finalFraction;
    do
    {
        i += increment;
        finalFraction.n = ratioFraction.n * i;
        finalFraction.d = ratioFraction.d * i;
    } while ((finalFraction.n < nominatorMin) || (finalFraction.d < denominatorMin));

    // Check that the values are in acceptable range
    ASSERT_RETVAL(finalFraction.n <= nominatorMax, Error::invalidParameter);
    ASSERT_RETVAL(finalFraction.d <= denominatorMax, Error::invalidParameter);

    // Nominator == PLLN
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, (finalFraction.n << RCC_PLLCFGR_PLLN_Pos));

    // Let's select PLLP = 2. Set PLLP
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk, ConvertPllp(2UL));

    // Since PLLP = 2, PLLM is straightforward to calculate: Denominator / 2.
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_Msk, (finalFraction.d >> 1UL));

    // Set the PLL source bit
    Utils::SetBit(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_Pos, (source == OscillatorType::highSpeed_external));

    // Let's multiply the nominator with the source frequency for calculating the final PLL frequency.
    finalFraction.n *= GetFrequency(source);

    // Calculate the final PLL frequency.
    clockFrequencies[static_cast<uint32_t>(OscillatorType::pll)] = finalFraction.n / finalFraction.d;

    // Divide the denominator by 2 to remove PLLP.
    finalFraction.d >>= 1UL;

    // Next calculate minimum PLLQ value.
    // fsecondary max is 48MHz.
    const uint32_t secondaryPllFrequencyMax = MHz(48UL);
    // PLLQ max is 15.
    const uint32_t pllqMax = 15UL;
    
    // Let's increment PLLQ until the frequency is in the correct range.
    uint32_t secondaryPllFrequency;
    uint32_t pllq = 1UL;
    do
    {
        ++pllq;
        secondaryPllFrequency = finalFraction.n / (finalFraction.d * pllq);
    } while ((secondaryPllFrequency > secondaryPllFrequencyMax) && (pllq < pllqMax));

    // Store the USB PLL register values only if the frequency is sensible.
    if (secondaryPllFrequency <= secondaryPllFrequencyMax)
    {
        Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_Msk, (pllq << RCC_PLLCFGR_PLLQ_Pos));

        // Store the final secondary PLL frequency.
        clockFrequencies[static_cast<uint32_t>(OscillatorType::pllSecondary)] = secondaryPllFrequency;
    }

    return error;
}

Error Clocks::ConfigurePllManually(OscillatorType source, pllRegisters_t const& registers)
{
    ASSERT_RETVAL((source == OscillatorType::highSpeed_internal) || (source == OscillatorType::highSpeed_external), Error::invalidParameter);

    // The values have following limitations:
    // PLLP = [2, 4, 6, 8]
    // PLLN >= 50, PLLN <= 432
    // PLLM >= 2, PLLM <= 63
    // PLLQ >= 2, PLLQ <= 15
    uint32_t finalPllp = ConvertPllp(registers.pllp);
    ASSERT_RETVAL(finalPllp != 4UL, Error::invalidParameter);

    // Calculate frequencies
    float vcoFrequency = static_cast<float>(GetFrequency(source)) * static_cast<float>(registers.plln)/static_cast<float>(registers.pllm);
    // The VCO frequency must be between 100MHz and 432MHz.
    ASSERT_RETVAL((vcoFrequency >= MHz(100UL)) && (vcoFrequency <= MHz(432UL)), Error::invalidParameter);

    float pllFrequency = static_cast<uint32_t>(vcoFrequency/static_cast<float>(registers.pllp) + 0.5f);
    // Maximum PLL frequency is 168MHz.
    ASSERT_RETVAL(pllFrequency <= MHz(168UL), Error::invalidParameter);
    
    float pllSecondaryFrequency = static_cast<uint32_t>(vcoFrequency/static_cast<float>(registers.pllq) + 0.5f);
    // Maximum secondary PLL frequency is 48MHz.
    ASSERT_RETVAL(pllSecondaryFrequency <= MHz(48UL), Error::invalidParameter);

    ASSERT_RETVAL((registers.plln >= 50UL) && (registers.plln <= 432UL), Error::invalidParameter);
    ASSERT_RETVAL((registers.pllm >= 2UL) && (registers.pllm <= 63UL), Error::invalidParameter);
    ASSERT_RETVAL((registers.pllq >= 2UL) && (registers.pllq <= 15UL), Error::invalidParameter);

    // Set the register values
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLP_Msk, (finalPllp << RCC_PLLCFGR_PLLP_Pos));
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLN_Msk, (registers.plln << RCC_PLLCFGR_PLLN_Pos));
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLM_Msk, registers.pllm);
    Utils::SetBits(RCC->PLLCFGR, RCC_PLLCFGR_PLLQ_Msk, (registers.pllq << RCC_PLLCFGR_PLLQ_Pos));

    // Set the PLL source bit
    Utils::SetBit(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_Pos, (source == OscillatorType::highSpeed_external));

    // Store frequencies
    clockFrequencies[static_cast<uint32_t>(OscillatorType::pll)] = pllFrequency;
    clockFrequencies[static_cast<uint32_t>(OscillatorType::pllSecondary)] = pllSecondaryFrequency;

    return Error::noErrors;
}

OscillatorType Clocks::GetPllSource(void)
{
    OscillatorType type;
    if (Utils::GetBit(RCC->PLLCFGR, RCC_PLLCFGR_PLLSRC_Pos) == true)
    {
        type = OscillatorType::highSpeed_external;
    }
    else
    {
        type = OscillatorType::highSpeed_internal;
    }
    return type;
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
            // Intentional fall-through.
        case OscillatorType::pllSecondary:
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
    ASSERT(type == OscillatorType::highSpeed_external);
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
    OscillatorType type = GetSysClockSource();
    return clockFrequencies[static_cast<uint32_t>(type)];
}

Error Clocks::SetSysClockSource(OscillatorType type)
{
    ASSERT_RETVAL((type == OscillatorType::highSpeed_internal)
               || (type == OscillatorType::highSpeed_external)
               || (type == OscillatorType::pll),
               Error::invalidParameter);

    Error error = Error::noErrors;

    if (IsRunning(type) == true)
    {
        if (GetSysClockSource() != type)
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
                    // This state should be already catched in sserts, but better to be safe.
                    isTimeout = false;
                    error = Error::invalidParameter;
                    break;
            }

            if (isTimeout)
            {
                error = Error::timeout;
            }
        }
    }
    else
    {
        error = Error::unavailableResource;
    }

    return error;
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

uint32_t ConvertPllp(uint32_t pllp)
{
    uint32_t registerValue;

    if (((pllp & 0x1UL) == 0UL) && (pllp <= 8UL))
    {
        registerValue = pllp >> 1UL;
        --registerValue;
    }
    else
    {
        registerValue = 4UL;
    }

    return registerValue;
}

}
