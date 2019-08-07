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

//! @file    Hal_Internal.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    27 Jul 2019
//!
//! @class   Internal
//! @brief   STM32F429ZI internal functionality
//! 
//! This is a generic class that covers ceratain STM32F429ZI-specific functionalities outside the generic HAL API.

#ifndef HAL_INTERNAL_HPP_
#define HAL_INTERNAL_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>
#include <stm32f429xx_mock.h>

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

//! @class   Internal
//! @brief   STM32F429ZI internal functionality
//! This is a generic class that covers ceratain STM32F429ZI-specific functionalities outside the generic HAL API.
class Internal
{
public:
    /// @brief Simple constructor.
    explicit Internal(void) {};

    /// @brief This function waits for a given bit to be set in given register.
    /// @param rccRegister - A reference to the RCC register to be checked.
    /// @param bit - A bit to be checked.
    /// @return Returns true if the bit is not set within timeout window. False if successful.
    static bool WaitForBitToSet(__IO uint32_t& rccRegister, uint32_t bit);

    /// @brief This function waits for a given bit to be cleared in given register.
    /// @param rccRegister - A reference to the RCC register to be checked.
    /// @param bit - A bit to be checked.
    /// @return Returns true if the bit is not cleared within timeout window. False if successful.
    static bool WaitForBitToClear(__IO uint32_t& rccRegister, uint32_t bit);

    /// @brief This function waits for a given bit patter to be set in given register.
    /// @param rccRegister - A reference to the RCC register to be checked.
    /// @param mask - The bitfield mask.
    /// @param pattern - The bitfield pattern.
    /// @return Returns true if the pattern is not set within timeout window. False if successful.
    static bool WaitForBitPatternToSet(__IO uint32_t& rccRegister, uint32_t mask, uint32_t pattern);

private:
    
};

} // namespace Hal

#endif // HAL_INTERNAL_HPP_
