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

//! @file    Utils_Assert_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mock for assert.
//! 
//! This is mock for assert used in ASch. It also has helper functions for unit tests for reading and resetting failure count.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Assert_Mock.hpp>
#include <Utils_Assert.hpp>


namespace
{

uint32_t failureCount = 0UL;    //!< Number of failed asserts.

}

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Member Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASchMock
{

Assert::Assert(void)
{
    return;
}

void Assert::Init(void)
{
    failureCount = 0UL;
    return;
}

uint32_t Assert::GetFails(void)
{
    return failureCount;
}

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

void Utils::AssertFailure(void)
{
    ++failureCount;
    return;
}
