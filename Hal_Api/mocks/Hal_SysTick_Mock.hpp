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

//! @file    Hal_SysTick_Mock.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for System HAL.
//! 
//! These are initialisation functions for mocks. The mocks are utilising FakeIt framework.

#ifndef HAL_SYSTICK_MOCK_HPP_
#define HAL_SYSTICK_MOCK_HPP_

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

//! @class SysTick
//! @brief This is a mock class for SysTick HAL
class SysTick
{
public:
    explicit SysTick(void) {};
    virtual void SetInterval(uint16_t intervalIn01Ms);
    virtual void Start(void);
    virtual void Stop(void);
    virtual bool IsRunning(void);
};

/// @brief The mock entity for accessing FakeIt interface.
extern Mock<SysTick> mockHalSysTick;

/// @brief This function initialises the HAL System mock.
void InitSysTick(void);

} // namespace HalMock

#endif // HAL_SYSTICK_MOCK_HPP_
