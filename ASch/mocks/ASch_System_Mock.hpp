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

//! @file    ASch_System_Mock.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for ASch System.
//! 
//! These are initialisation functions for mocks. The mocks are utilising FakeIt framework.

#ifndef ASCH_SYSTEM_MOCK_HPP_
#define ASCH_SYSTEM_MOCK_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Framework Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <catch.hpp>
#include <fakeit.hpp>
using namespace fakeit;

#include <ASch_System.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Init Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASchMock
{

//! @class System
//! @brief This is a mock class for ASch System
class System
{
public:
    explicit System(void) {};
    virtual void Error(ASch::SysError error);
    virtual void EnableResetOnSystemError(void);
    virtual void Init(void);
    virtual void PreStartConfig(void);
};

/// @brief The mock entity for accessing FakeIt interface.
extern Mock<System> mockASchSystem;

/// @brief This function initialises the ASch System mock.
void InitSystem(void);

} // namespace ASchMock

#endif // ASCH_SYSTEM_MOCK_HPP_
