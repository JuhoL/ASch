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
//! @brief   This is a clocks HAL interface.
//! 
//! This class handles clock configurations.

#ifndef HAL_CLOCKS_HPP_
#define HAL_CLOCKS_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>
#include <Hal_Common.hpp>
#include <Hal_ClockTypes.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

constexpr uint32_t MHz(uint32_t MHz) {return MHz * 1000000UL;}

} // namespace Hal

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
    /// @return Returns an error code if operation fails.
    static Error Enable(OscillatorType type);

    /// @brief Disables the given oscillator.
    /// @param type - The oscillator to be disabled.
    /// @return Returns an error code if operation fails.
    static Error Disable(OscillatorType type);

    /// @brief Configures PLL.
    /// Note that this does not start the PLL yet!
    /// @param source - The oscillator to be used as source. Note: Make sure you have configured a frequency to the source!
    /// @param frequency - Desired PLL target frequency.
    /// @return Returns an error code if operation fails.
    static Error ConfigurePll(OscillatorType source, uint32_t frequency);

    /// @brief Checks if the given oscillator is running.
    /// @return Returns true if the oscillator is running.
    static bool IsRunning(OscillatorType type);

    /// @brief Sets the frequency of the given clock.
    /// This frequency is used for calculations only! It's used for calculating timers delays, baudrates, etc.
    /// The FW has no ability to validate the correctness of this value, so be careful to configure it correctly.
    /// @param type - The oscillator which frequency will be set.
    /// @param frequency - Clock frequency in Hz.
    static void SetFrequency(OscillatorType type, uint32_t frequency);

    /// @brief Returns the frequency of the given clock.
    /// @return Clock frequency in Hz. If the clock is off, 0Hz shall be returned.
    static uint32_t GetFrequency(OscillatorType type);

    /// @brief Returns current system clock frequency.
    /// @return Current system clock frequency in Hz.
    static uint32_t GetSysClockFrequency(void);

    /// @brief Sets system clock source.
    /// @param type - The oscillator type to be set as system clock source.
    /// @return Returns an error code if operation fails.
    static Error SetSysClockSource(OscillatorType type);

    /// @brief Reads system clock source.
    /// @return The oscillator type set as system clock source.
    static OscillatorType GetSysClockSource(void);

private:
    static uint32_t sysClockFrequency;
    static uint32_t clockFrequencies[static_cast<std::size_t>(OscillatorType::unknown)];
};

} // namespace Hal

#endif // HAL_CLOCKS_HPP_
