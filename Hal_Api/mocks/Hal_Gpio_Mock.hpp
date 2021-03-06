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

//! @file    Hal_Gpio_Mock.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for GPIO HAL.
//! 
//! These are initialisation functions for mocks. The mocks are utilising FakeIt framework.

#ifndef HAL_GPIO_MOCK_HPP_
#define HAL_GPIO_MOCK_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Framework Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <catch.hpp>
#include <fakeit.hpp>
using namespace fakeit;

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Init Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

namespace HalMock
{

//! @class Gpio
//! @brief This is a mock class for GPIO HAL
class Gpio
{
public:
    explicit Gpio(void) {};
    virtual void GetConfiguration(gpioConfig_t& gpio);
    virtual void SetConfiguration(gpioConfig_t& gpio);
    virtual void SetOutputState(Pin_t& pin, bool state);
    virtual bool GetInputState(Pin_t& pin);
    virtual bool GetOutputState(Pin_t& pin);
};

extern Mock<Gpio> mockHalGpio;

/// @brief This function initialises the HAL Gpio mock.
void InitGpio(void);

} // namespace HalMock

#endif // HAL_GPIO_MOCK_HPP_
