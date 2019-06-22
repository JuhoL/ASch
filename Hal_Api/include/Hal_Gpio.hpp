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

//! @file    Hal_Gpio.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    16 Jun 2019
//!
//! @class   Gpio
//! @brief   This is a GPIO HAL interface.
//! 
//! This class handles GPIO configuration and IO access.

#ifndef HAL_GPIO_HPP_
#define HAL_GPIO_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>
#include <Hal_GpioPins.hpp>
#include <Hal_GpioAlternateFunctions.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

/// @brief This is the GPIO mode enum.
enum class GpioMode
{
    input = 0,  //!< The GPIO is an input.
    output,     //!< The GPIO is an output.
    alternate,  //!< The GPIO is in alternate function mode defined by AlternateFunction enum.
    analog      //!< The GPIO is in ADC/DAC mode.
};

/// @brief This is the GPIO output speed enum.
enum class GpioSpeed
{
    low = 0,    //!< Low speed (power save)
    medium,     //!< Medium speed
    high,       //!< High speed
    veryHigh    //!< Very high speed
};

/// @brief This is the GPIO pull-up/pull-down enum.
enum class GpioPull
{
    floating = 0,   //!< The IO is floating.
    pullUp,         //!< The IO is pulled up.
    pullDown        //!< The IO is pulled down.
};

/// @brief This is the GPIO configuration struct.
typedef struct
{
    Pin_t pin;                              //!< GPIO pin to be configured.
    GpioMode mode;                          //!< Pin mode.
    bool isOpenDrain;                       //!< Set true to enable open drain output. No effect on inputs.
    GpioSpeed speed;                        //!< Pin speed.
    GpioPull pull;                          //!< Pin pull-up/pull-down configuration.
    AlternateFunction alternateFunction;    //!< Alternate function selector. No effet if alternate function mode is not enabled.
} gpioConfig_t;

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

//! @class Gpio
//! @brief This is a GPIO HAL interface.
//! This class handles GPIO configuration and IO access.
class Gpio
{
public:
    /// @brief Simple constructor.
    explicit Gpio(void);

    /// @brief This function gets the configuration of the given pin.
    /// The pin is selected by configuring the pin in the configuration struct before passing it to this function. 
    ///
    /// @param gpio - A reference to a GPIO configuration struct.
    static_mf void GetConfiguration(gpioConfig_t& gpio);

    /// @brief This function sets the configuration of the given pin.
    /// @param gpio - A reference to a GPIO configuration struct.
    static_mf void SetConfiguration(gpioConfig_t& gpio);

    /// @brief This function sets the output state of the given pin.
    /// @param pin - A reference to the pin selector.
    /// @param state - A new pin state.
    static_mf void SetOutputState(Pin_t& pin, bool state);

    /// @brief This function gets the input state of the given pin.
    /// @param pin - A reference to the pin selector.
    /// @return The state of the pin input.
    static_mf bool GetInputState(Pin_t& pin);

    /// @brief This function gets the output state of the given pin.
    /// @param pin - A reference to the pin selector.
    /// @return The state of the pin output.
    static_mf bool GetOutputState(Pin_t& pin);

    /// @brief This function locks the given port preventing further configuration. The lock will be reset only via MCU reset.
    /// @param port - Port to be locked.
    static_mf void LockPort(Port port);

private:
    /// @brief This function gets the mode of the given pin.
    /// @param pin - A reference to the pin selector.
    /// @return The mode of the pin.
    static GpioMode GetMode(Pin_t& pin);

    /// @brief This function gets the open drain configuration of the given pin.
    /// @param pin - A reference to the pin selector.
    /// @return The open drain configuration of the pin.
    static bool IsOpenDrain(Pin_t& pin);

    /// @brief This function gets the speed of the given pin.
    /// @param pin - A reference to the pin selector.
    /// @return The speed of the pin.
    static GpioSpeed GetSpeed(Pin_t& pin);

    /// @brief This function gets the pull-up/pull-down configuration of the given pin.
    /// @param pin - A reference to the pin selector.
    /// @return The pull-up/pull-down configuration of the pin.
    static GpioPull GetPull(Pin_t& pin);

    /// @brief This function gets the alternate function configuration of the given pin.
    /// @param pin - A reference to the pin selector.
    /// @return The alternate function configuration of the pin.
    static AlternateFunction GetAlternateFunction(Pin_t& pin);
};

} // namespace Hal

#endif // HAL_GPIO_HPP_
