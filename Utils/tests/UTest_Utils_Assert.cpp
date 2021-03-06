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

//! @file    UTest_Utils_Assert.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    22 Jun 2019
//! 
//! @brief   These are unit tests for Utils_Assert.cpp
//! 
//! These are unit tests for Utils_Assert.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Catch_Utils.hpp>

#include <Utils_Assert.hpp>
#include <ASch_System_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Assert passes", "[assert]")
{
    ASchMock::InitSystem();

    GIVEN ("a condition passes")
    {
        bool condition = 1 == 1;

        WHEN ("passing the condition to assert")
        {
            bool isFail = Utils::Assert(condition);

            THEN ("assert failure shall not trigger")
            {
                REQUIRE_CALLS (0, ASchMock::mockASchSystem, Error);
            }
            AND_THEN ("assert shall return false")
            {
                REQUIRE (isFail == false);
            }
        }
    }
}

SCENARIO ("Assert fails", "[assert]")
{
    ASchMock::InitSystem();
    
    GIVEN ("a condition fails")
    {
        bool condition = 1 == 0;

        WHEN ("passing the condition to assert")
        {
            bool isFail = Utils::Assert(condition);

            THEN ("assert failure shall trigger")
            {
                REQUIRE_PARAM_CALLS (1, ASchMock::mockASchSystem, Error, ASch::SysError::assertFailure);
            }
            AND_THEN ("assert shall return true")
            {
                REQUIRE (isFail == true);
            }
        }
    }
}
