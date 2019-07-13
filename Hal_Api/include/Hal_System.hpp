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

//! @file    Hal_System.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    28 Oct 2018
//!
//! @class   System
//! @brief   This is HAL interface for system level functionality.

#ifndef HAL_SYSTEM_HPP_
#define HAL_SYSTEM_HPP_

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

//! @class System
//! @brief This is HAL interface for system level functionality.
//! This class handler system clocks and power management.
class System
{
public:
    /// @brief Simple constructor.
    explicit System(void);

    /// @brief Puts the system into sleep.
    static void Sleep(void);

    /// @brief Wakes the system up from sleep.
    static void WakeUp(void);
    
    /// @brief Initialises power control.
    static void InitPowerControl(void);
    
    /// @brief Initialises system clocks.
    static void InitClocks(void);

    /// @brief Performs MCU reset.
    static void Reset(void);

    /// @brief Critical system error handler. Halts the CPU.
    static void CriticalSystemError(void);

private:
    
};

} // namespace Hal

#endif // HAL_SYSTEM_HPP_
