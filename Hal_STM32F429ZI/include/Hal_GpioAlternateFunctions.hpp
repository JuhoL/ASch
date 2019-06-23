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

//! @file    Hal_GpioAlternateFunctions.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    16 Jun 2019
//!
//! @brief   This header defines GPIO alternate function enum for STM32F429ZI
//! 
//! This enum is needed by GPIO HAL.

#ifndef HAL_GPIO_ALTERNATE_FUNCTIONS_HPP_
#define HAL_GPIO_ALTERNATE_FUNCTIONS_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

// See STM32F429ZI datasheet (ST DocID024030 Table 12) for the pin routings
enum class AlternateFunction
{
    sys1 = 0,

    tim_1,
    tim_2,
    tim_3,
    tim_4,
    tim_5,
    tim_8,
    tim_9,
    tim_10,
    tim_11,
    tim_12,
    tim_13,
    tim_14,
    
    i2c_1,
    i2c_2,
    i2c_3,
    
    spi_1,
    spi_2,
    spi_3,
    spi_4,
    spi_5,
    spi_6,
    spi_7,
    
    sai_1,
    
    usart_1,
    usart_2,
    usart_3,
    usart_4,
    usart_5,
    usart_6,
    usart_7,
    usart_8,

    can_1,
    can_2,

    lcd,

    otg1_fs,
    otg2_fs,
    otg2_hs,

    eth,

    fmc,

    sdio,

    dcmi,

    eventOut
};

} // namespace Hal

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

#endif // HAL_GPIO_ALTERNATE_FUNCTIONS_HPP_
