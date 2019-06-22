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
//! @brief   !!!!! Brief file description here !!!!!
//! 
//! !!!!! Detailed file description here !!!!!

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

enum class GpioMode
{
    input = 0,
    output,
    alternate,
    analog
};

enum class GpioSpeed
{
    low = 0,
    medium,
    high,
    veryHigh
};

enum class GpioPull
{
    floating = 0,
    pullUp,
    pullDown
};

enum class GpioPin
{
    pa0 = 0,
    pa1,
};

typedef struct
{
    Pin_t pin;
    GpioMode mode;
    bool isOpenDrain;
    GpioSpeed speed;
    GpioPull pull;
    AlternateFunction alternateFunction;
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

/// @class Gpio
class Gpio
{
public:
    explicit Gpio(void);

    static_mf void GetConfiguration(gpioConfig_t& gpio);
    static_mf void SetConfiguration(gpioConfig_t& gpio);

    static_mf void SetOutputState(Pin_t& pin, bool state);
    static_mf bool GetInputState(Pin_t& pin);
    static_mf bool GetOutputState(Pin_t& pin);

    static_mf void LockPort(Port port);

private:
    static GpioMode GetMode(Pin_t& pin);
    static bool IsOpenDrain(Pin_t& pin);
    static GpioSpeed GetSpeed(Pin_t& pin);
    static GpioPull GetPull(Pin_t& pin);
    static AlternateFunction GetAlternateFunction(Pin_t& pin);
};

} // namespace Hal

#endif // HAL_GPIO_HPP_
