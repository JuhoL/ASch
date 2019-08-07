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

//! @file    Hal_Internal.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    27 Jul 2019
//!
//! @class   Internal
//! @brief   !!!!! Brief file description here !!!!!
//! 
//! !!!!! Detailed file description here !!!!!

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Internal.hpp>
#include <Utils_Bit.hpp>
#include <Hal_Clocks.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Static Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 6. Class Member Definitions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

bool Internal::WaitForBitToSet(__IO uint32_t& rccRegister, uint32_t bit)
{
    // Since it's fair to assume that when configuring clocks no timers or other peripherals are available,
    // a simple downcounter shall be used with preset value based on current system clock. This provides
    // roughly the same scale timeout regardless of the current system clock.
    uint32_t timeout = Hal::Clocks::GetSysClockFrequency() >> 1UL;
    while ((Utils::GetBit(rccRegister, bit) == false) && (timeout > 0UL))
    {
        --timeout;
    }
    return timeout == 0UL;
}

bool Internal::WaitForBitToClear(__IO uint32_t& rccRegister, uint32_t bit)
{
    // Since it's fair to assume that when configuring clocks no timers or other peripherals are available,
    // a simple downcounter shall be used with preset value based on current system clock. This provides
    // roughly the same scale timeout regardless of the current system clock.
    uint32_t timeout = Hal::Clocks::GetSysClockFrequency() >> 1UL;
    while ((Utils::GetBit(rccRegister, bit) == true) && (timeout > 0UL))
    {
        --timeout;
    }
    return timeout == 0UL;
}

bool Internal::WaitForBitPatternToSet(__IO uint32_t& rccRegister, uint32_t mask, uint32_t pattern)
{
    // Since it's fair to assume that when configuring clocks no timers or other peripherals are available,
    // a simple downcounter shall be used with preset value based on current system clock. This provides
    // roughly the same scale timeout regardless of the current system clock.
    uint32_t timeout = Hal::Clocks::GetSysClockFrequency() >> 1UL;
    while ((Utils::CompareBits(rccRegister, mask, pattern) == false) && (timeout > 0UL))
    {
        --timeout;
    }
    return timeout == 0UL;
}

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------

