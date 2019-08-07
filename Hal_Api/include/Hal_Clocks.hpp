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

//! @file    Hal_Clocks.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    25 Jul 2019
//!
//! @class   Clocks
//! @brief   !!!!! Brief file description here !!!!!
//! 
//! !!!!! Detailed file description here !!!!!

#ifndef HAL_CLOCKS_HPP_
#define HAL_CLOCKS_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

/// @brief This is a struct that is used to keep track of task states.
enum class OscillatorType
{
    highSpeed_internal = 0,
    highSpeed_external,
    lowSpeed_internal,
    lowSpeed_external,
    pll,
    unknown
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

namespace Hal
{

//! @class   Clocks
//! @brief   !!!!! Brief file description here !!!!!
//! !!!!! Detailed file description here !!!!!
class Clocks
{
public:
    /// @brief Simple constructor.
    explicit Clocks(void) {};

    /// @brief Enables the given oscillator.
    /// @param type - The oscillator to be enabled.
    static void Enable(Hal::OscillatorType type);

    /// @brief Disables the given oscillator.
    /// @param type - The oscillator to be disabled.
    static void Disable(Hal::OscillatorType type);

    /// @brief Checks if the given oscillator is running.
    /// @return Returns true if the oscillator is running.
    static bool IsRunning(Hal::OscillatorType type);

    /// @brief Returns current system clock frequency.
    /// @return Current system clock frequency in Hz.
    static uint32_t GetSysClockFrequency(void);

    /// @brief Sets system clock source.
    /// @param type - The oscillator type to be set as system clock source.
    static void SetSysClockSource(Hal::OscillatorType type);

    /// @brief Reads system clock source.
    /// @return The oscillator type set as system clock source.
    static Hal::OscillatorType GetSysClockSource(void);

private:
    static uint32_t sysClockFrequency;
};

} // namespace Hal

#endif // HAL_CLOCKS_HPP_
