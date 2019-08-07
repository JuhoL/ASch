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

//! @file    Hal_Internal_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for HAL GPIO.
//! 
//! These are mocks for HAL GPIO utilising FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Internal_Mock.hpp>
#include <Hal_Internal.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Initialisation
//-----------------------------------------------------------------------------------------------------------------------------

namespace HalMock
{

Mock<Internal> mockHalInternal;
static HalMock::Internal& internal = mockHalInternal.get();

void InitInternal(void)
{
    static bool isFirstInit = true;

    if (isFirstInit == true)
    {
        Fake(Method(mockHalInternal, WaitForBitToSet));
        Fake(Method(mockHalInternal, WaitForBitToClear));
        Fake(Method(mockHalInternal, WaitForBitPatternToSet));

        isFirstInit = false;
    }
    else
    {
        mockHalInternal.Reset();
    }
    return;
}

} // namespace HalMock

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

bool Internal::WaitForBitToSet(__IO uint32_t& rccRegister, uint32_t bit)
{
    return HalMock::internal.WaitForBitToSet(rccRegister, bit);
}

bool Internal::WaitForBitToClear(__IO uint32_t& rccRegister, uint32_t bit)
{
    return HalMock::internal.WaitForBitToClear(rccRegister, bit);
}

bool Internal::WaitForBitPatternToSet(__IO uint32_t& rccRegister, uint32_t mask, uint32_t pattern)
{
    return HalMock::internal.WaitForBitPatternToSet(rccRegister, mask, pattern);
}

} // namespace Hal

