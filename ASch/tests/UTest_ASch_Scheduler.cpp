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

//! @file    UTest_ASch_Scheduler.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    22 Aug 2018
//! 
//! @brief   These are unit tests for ASch_Scheduler.cpp
//! 
//! These are unit tests for ASch_Scheduler.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <fakeit.hpp>
using namespace fakeit;

#include <ASch_Scheduler.hpp>
#include <Hal_SysTick.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Developer starts a scheduler", "[scheduler]")
{
    GIVEN ("a scheduler is not yet created")
    {
        WHEN ("a scheduler is created")
        {
            Mock<Hal::SysTick> mockSysTick;
            Fake(Method(mockSysTick, SetInterval));

            ASch::Scheduler(mockSysTick.get(), 1U);

            THEN ("the scheduler shall configure system tick")
            {
                REQUIRE_NOTHROW (Verify(Method(mockSysTick, SetInterval)).Exactly(1));
            }
            AND_THEN ("scheduler tick handler shall be set as system tick handler")
            {
                REQUIRE (1 == 1);
            }
            AND_THEN ("no tasks shall be running")
            {
                REQUIRE (1 == 1);
            }
        }
    }
}
