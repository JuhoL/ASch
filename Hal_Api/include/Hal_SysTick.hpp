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

//! @file    Hal_SysTick.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    22 Aug 2018
//!
//! @class   SysTick
//! @brief   HAL interface for SysTick
//! 
//! This module configures SysTick peripheral that is used by the Scheduler module.

#ifndef HAL_SYSTICK_HPP_
#define HAL_SYSTICK_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

//! @class SysTick
//! @brief HAL interface for SysTick
//! This class configures and controls SysTick peripheral that is used by the Scheduler module.
class SysTick
{
public:
    /// @brief Simple constructor.
    explicit SysTick(void);
    
    /// @brief Sets the SysTick interval.
    /// @param intervalIn01Ms - Interval in 0.1ms (i.e 10 = 1.0ms)
    static_mf void SetInterval(uint16_t intervalIn01Ms);
    
    /// @brief Starts the SysTick.
    static_mf void Start(void);
    
    /// @brief Stops the SysTick.
    static_mf void Stop(void);

private:
    
};

} // namespace Hal

#endif // HAL_SYSTICK_HPP_
