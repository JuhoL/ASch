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

//! @file    Hal_ClockTypes.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    08 Aug 2019
//!
//! @class   ClockTypes
//! @brief   Enum and struct definitions used in Hal_Clock.
//! 
//! This file is required by HAL API. The enums and structs depend on the MCU type.

#ifndef HAL_CLOCKTYPES_HPP_
#define HAL_CLOCKTYPES_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

/// @brief This is a enum defining available clocks in STM32F429ZI.
enum class OscillatorType
{
    highSpeed_internal = 0, //!< HSI
    highSpeed_external,     //!< HSE
    lowSpeed_internal,      //!< LSI
    lowSpeed_external,      //!< LSE
    pll,                    //!< PLL
    pllSecondary,           //!< Secondary PLL to feed USB, SDIO, and RNG.
    unknown                 //!< Invalid clock source.
};

} // namespace Hal

#endif // HAL_CLOCKTYPES_HPP_
