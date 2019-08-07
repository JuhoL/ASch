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

//! @file    Hal_SysTick.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    22 Aug 2018
//!
//! @class   SysTick
//! @brief   HAL interface for SysTick
//! 
//! This module configures SysTick peripheral that is used by the Scheduler module.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_SysTick.hpp>
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
/// @brief Time value of calibration register in 0.1 ms resolution.
/// On ARM Cortex core, the SysTick calibration value corresponds 10.0 ms tick.
const uint32_t calibratedIntervalIn01Ms = 100UL;

const uint32_t calibrationMask = SysTick_CALIB_TENMS_Msk; //!< Mask for CALIB register to get the calibration value.
}
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

SysTick::SysTick(void)
{
    return;
}

void SysTick::SetInterval(uint16_t intervalIn01Ms)
{
    ASSERT(intervalIn01Ms > 0U);
    ASSERT(Utils::GetBit(SYSTICK->CTRL, SysTick_CTRL_ENABLE_Pos) == false);

    uint32_t calibrationValue = SYSTICK->CALIB & calibrationMask;
    ASSERT(calibrationValue > 0U); // In some rare cases the calibration can be zero.

    // Simple claculation PreLoad = interval / intervalOfCalibration * calibration with rounding.
    uint32_t preLoad = ((calibrationValue * static_cast<uint32_t>(intervalIn01Ms)) + (calibratedIntervalIn01Ms >> 1UL)) / calibratedIntervalIn01Ms;
    
    // The correct preload value is "desired interval - 1".
    if (preLoad > 0UL)
    {
        --preLoad;
    }

    SYSTICK->LOAD = preLoad;
    return;
}

void SysTick::Start(void)
{
    Utils::SetBit(SYSTICK->CTRL, SysTick_CTRL_ENABLE_Pos, true);
    return;
}

void SysTick::Stop(void)
{
    Utils::SetBit(SYSTICK->CTRL, SysTick_CTRL_ENABLE_Pos, false);
    return;
}

bool SysTick::IsRunning(void)
{
    return Utils::GetBit(SYSTICK->CTRL, SysTick_CTRL_ENABLE_Pos);
}

} // namespace Hal

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------
