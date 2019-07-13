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

//! @file    Hal_Isr_Mock.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for ISR HAL.
//! 
//! These are initialisation functions for mocks. The mocks are utilising FakeIt framework.

#ifndef HAL_ISR_MOCK_HPP_
#define HAL_ISR_MOCK_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Framework Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <catch.hpp>
#include <fakeit.hpp>
using namespace fakeit;

#include <Hal_Isr.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Init Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

namespace HalMock
{

//! @class Isr
//! @brief This is a mock class for HAL Isr
class Isr
{
public:
    explicit Isr(void) {};
    virtual void Init(void);
    virtual void EnableGlobal(void);
    virtual void DisableGlobal(void);
    virtual void SetHandler(Hal::Interrupt type, Hal::interruptHandler_t Handler);
    virtual Hal::interruptHandler_t GetHandler(Hal::Interrupt type);
    virtual void Enable(Hal::Interrupt type);
    virtual void Disable(Hal::Interrupt type);
    virtual void SetPriority(Hal::Interrupt type, uint32_t priority);
    virtual uint8_t GetPriority(Hal::Interrupt type);
    virtual void SetPending(Hal::Interrupt type);
    virtual bool GetPending(Hal::Interrupt type);
    virtual void Clear(Hal::Interrupt type);
};

/// @brief The mock entity for accessing FakeIt interface.
extern Mock<Isr> mockHalIsr;

/// @brief This function initialises the HAL System mock.
void InitIsr(void);

} // namespace HalMock

#endif // HAL_ISR_MOCK_HPP_
