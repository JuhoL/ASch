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

//! @file    UTest_Hal_System.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    28 Oct 2018
//! 
//! @brief   These are unit tests for Hal_System.cpp
//! 
//! These are unit tests for Hal_System.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Catch_Utils.hpp>

#include <Hal_System.hpp>
#include <Utils_Bit.hpp>
#include <stm32f429xx_mock.h>
#include <Utils_Assert_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("The power control module is configured", "[hal_system]")
{
    Hal_Mock::InitRccRegisters();

    GIVEN ("a HAL System class is created")
    {
        Hal::System system = Hal::System();

        WHEN ("the power control is initialised")
        {
            system.InitPowerControl();

            THEN ("PWREN bit in APB1ENR register shall be set")
            {
                REQUIRE (RCC->APB1ENR == Utils::Bit(RCC_APB1ENR_PWREN_Pos));
            }
        }
    }
}

SCENARIO ("A MCU must be reset", "[hal_system]")
{
    Hal_Mock::InitScbRegisters();

    GIVEN ("a HAL System class is created")
    {
        Hal::System system = Hal::System();

        WHEN ("the reset function is called")
        {
            system.Reset();

            THEN ("SYSRESETREQ bit in AIRCR register shall be set")
            {
                REQUIRE (SCB->AIRCR == Utils::Bit(SCB_AIRCR_SYSRESETREQ_Pos));
            }
        }
    }
}
