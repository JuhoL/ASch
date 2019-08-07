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

//! @file    UTest_Hal_Internal.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    27 Jul 2019
//! 
//! @brief   These are unit tests for Hal_Internal.cpp
//! 
//! These are unit tests for Hal_Internal.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Catch_Utils.hpp>

#include <Hal_Internal.hpp>
#include <stm32f429xx_mock.h>
#include <Utils_Bit.hpp>
#include <Hal_Clocks_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Waiting for bit to be set in a register", "[hal_internal]")
{
    Hal_Mock::InitRccRegisters();
    HalMock::InitClocks();

    GIVEN ("s bit is set and system frequency is 16MHz")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, true);
        SET_RETURN(HalMock::mockHalClocks, GetSysClockFrequency, 16000000UL);

        WHEN ("waiting for the bit to set")
        {
            bool isTimeout = Hal::Internal::WaitForBitToSet(RCC->CR, RCC_CR_HSIRDY_Pos);
            THEN ("the timeout shall be false")
            {
                REQUIRE (isTimeout == false);
            }
        }
    }

    GIVEN ("s bit is not set and system frequency is 16MHz")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, false);
        SET_RETURN(HalMock::mockHalClocks, GetSysClockFrequency, 16000000UL);

        WHEN ("waiting for the bit to set")
        {
            bool isTimeout = Hal::Internal::WaitForBitToSet(RCC->CR, RCC_CR_HSIRDY_Pos);
            THEN ("the timeout shall be true")
            {
                REQUIRE (isTimeout == true);
            }
        }
    }
}

SCENARIO ("Waiting for bit to be cleared in a register", "[hal_internal]")
{
    Hal_Mock::InitRccRegisters();
    HalMock::InitClocks();

    GIVEN ("s bit is not set and system frequency is 16MHz")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, false);
        SET_RETURN(HalMock::mockHalClocks, GetSysClockFrequency, 16000000UL);

        WHEN ("waiting for the bit to set")
        {
            bool isTimeout = Hal::Internal::WaitForBitToClear(RCC->CR, RCC_CR_HSIRDY_Pos);
            THEN ("the timeout shall be false")
            {
                REQUIRE (isTimeout == false);
            }
        }
    }

    GIVEN ("s bit is set and system frequency is 16MHz")
    {
        Utils::SetBit(RCC->CR, RCC_CR_HSIRDY_Pos, true);
        SET_RETURN(HalMock::mockHalClocks, GetSysClockFrequency, 16000000UL);

        WHEN ("waiting for the bit to set")
        {
            bool isTimeout = Hal::Internal::WaitForBitToClear(RCC->CR, RCC_CR_HSIRDY_Pos);
            THEN ("the timeout shall be true")
            {
                REQUIRE (isTimeout == true);
            }
        }
    }
}

SCENARIO ("Waiting for bit pattern to be set in a register", "[hal_internal]")
{
    Hal_Mock::InitRccRegisters();
    HalMock::InitClocks();

    GIVEN ("SW bits in RCC CFGR are set to 01 and system frequency is 16MHz")
    {
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_0);
        SET_RETURN(HalMock::mockHalClocks, GetSysClockFrequency, 16000000UL);

        WHEN ("waiting for SW bits in RCC CFGR to be set to 01")
        {
            bool isTimeout = Hal::Internal::WaitForBitPatternToSet(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_0);
            THEN ("the timeout shall be false")
            {
                REQUIRE (isTimeout == false);
            }
        }
    }

    GIVEN ("SW bits in RCC CFGR are set to 10 and system frequency is 16MHz")
    {
        Utils::SetBits(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_1);
        SET_RETURN(HalMock::mockHalClocks, GetSysClockFrequency, 16000000UL);

        WHEN ("waiting for SW bits in RCC CFGR to be set to 01")
        {
            bool isTimeout = Hal::Internal::WaitForBitPatternToSet(RCC->CFGR, RCC_CFGR_SW_Msk, RCC_CFGR_SW_0);
            THEN ("the timeout shall be true")
            {
                REQUIRE (isTimeout == true);
            }
        }
    }
}
