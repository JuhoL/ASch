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

//! @file    UTest_ASch_System.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    20 Aug 2018
//! 
//! @brief   These are unit tests for ASch_System.cpp
//! 
//! These are unit tests for ASch_System.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Catch_Utils.hpp>

#define SYSTEM_UNIT_TEST    // For enabling test functions in ASch_TestConfiguration.hpp

#include <Hal_System_Mock.hpp>
#include <ASch_Scheduler_Mock.hpp>

#include <ASch_System.hpp>
#include <ASch_Configuration.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

// Test function declarations. See ASch_TestConfiguration.hpp for prototypes.
uint8_t preStartConfigCallCount[ASch::preStartConfigurationFunctionsMax] = {0U};

void InitConfigCallCounts(void)
{
    for (std::size_t i = 0; i < ASch::preStartConfigurationFunctionsMax; ++i)
    {
        preStartConfigCallCount[i] = 0U;
    }

    return;
}

} // anonymous namespace

namespace ASch
{

void PreStartConfig0(void)
{
    ++preStartConfigCallCount[0];
    return;
}

void PreStartConfig1(void)
{
    ++preStartConfigCallCount[1];
    return;
}

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("A system is configured", "[system]")
{
    ASchMock::InitScheduler();
    InitConfigCallCounts();
    
    GIVEN ("a system is not yet initialised")
    {
        WHEN ("initialisation function is called")
        {
            ASch::System::Init();

            THEN ("scheduler shall be initialised with configured tick value")
            {
                REQUIRE_PARAM_CALLS (1, ASchMock::mockASchScheduler, Init, ASch::Config::schedulerTickInterval);
            }
        }
    }
    
    GIVEN ("a system is not yet initialised")
    {
        WHEN ("pre-start config is called")
        {
            ASch::System::PreStartConfig();

            THEN ("all pre-start config functions shall be called once")
            {
                REQUIRE (preStartConfigCallCount[0] == 1U);
                REQUIRE (preStartConfigCallCount[1] == 1U);
            }
        }
    }
}

