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

//! @file    UTest_ASch_Main.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    13 May 2019
//! 
//! @brief   These are unit tests for ASch_Main.cpp
//! 
//! These are unit tests for ASch_Main.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Catch_Utils.hpp>

#include <ASch_Scheduler_Mock.hpp>
#include <ASch_System_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

}

extern int main_test(void);

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("System starts", "[feature_tag]")
{
    ASchMock::InitScheduler();
    ASchMock::InitSystem();

    GIVEN ("system is starting")
    {
        WHEN ("main() is called")
        {
            main_test();

            THEN ("system init, pre-start config, scheduler start shall be called and main loop shall be entered")
            {
                REQUIRE_CALLS (1, ASchMock::mockASchSystem, Init);
                REQUIRE_CALLS (1, ASchMock::mockASchSystem, PreStartConfig);
                REQUIRE_CALLS (1, ASchMock::mockASchScheduler, Start);
                REQUIRE_CALLS (1, ASchMock::mockASchScheduler, MainLoop);
                REQUIRE_CALL_ORDER (CALL(ASchMock::mockASchSystem, Init) + CALL(ASchMock::mockASchSystem, PreStartConfig) + CALL(ASchMock::mockASchScheduler, Start) + CALL(ASchMock::mockASchScheduler, MainLoop));
            }
        }
    }
}
