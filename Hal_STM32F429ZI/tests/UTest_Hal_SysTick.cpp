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

//! @file    UTest_Hal_SysTick.cpp
//! @author  Juho Lepistö juho.lepisto(a)gmail.com
//! @date    22 Aug 2018
//! 
//! @brief   These are unit tests for Hal_SysTick.cpp
//! 
//! These are unit tests for Hal_SysTick.cpp utilising Catch2 and FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Catch_Utils.hpp>

#include <Hal_SysTick.hpp>
#include <stm32f429xx_mock.h>
#include <Utils_Assert_Mock.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{
const uint32_t calibrationMask = 0xFFFFFFUL; //!< Mask for CALIB register to get the calibration value.
#define CALIB   (SYSTICK->CALIB & calibrationMask)
}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Developer configures SysTick", "[SysTick]")
{
    Hal_Mock::InitSysTickRegisters();

    GIVEN ("a SysTick instance is created")
    {
        Hal::SysTick tick = Hal::SysTick();

        WHEN ("interval is configured to 10 ms")
        {
            tick.SetInterval(100U);

            THEN ("SysTick LOAD register shall be set the same as CALIB")
            {
                // On ARM Cortex core, the calibration register contains a prescale value that produces 10ms tick.
                REQUIRE (SYSTICK->LOAD == (CALIB - 1UL));
            }
        }
        AND_WHEN ("interval is configured to 1 ms")
        {
            tick.SetInterval(10U);

            THEN ("SysTick LOAD register shall be set tenth of CALIB")
            {
                REQUIRE (SYSTICK->LOAD == ((CALIB/10UL) - 1UL));
            }
        }
        AND_WHEN ("interval is configured to 1 ms")
        {
            tick.SetInterval(10U);

            THEN ("SysTick LOAD register shall be set tenth of CALIB")
            {
                REQUIRE (SYSTICK->LOAD == ((CALIB/10UL) - 1UL));
            }
        }
    }
}

SCENARIO ("Developer configures SysTick wrong", "[SysTick]")
{
    Hal_Mock::InitSysTickRegisters();
    ASchMock::Assert::Init();

    GIVEN ("SysTick is stopped")
    {
        Hal::SysTick tick = Hal::SysTick();

        WHEN ("interval is configured to 0 ms")
        {
            tick.SetInterval(0U);

            THEN ("a critical system error shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }

    GIVEN ("SysTick is running")
    {
        Hal::SysTick tick = Hal::SysTick();
        tick.SetInterval(100U);
        tick.Start();

        WHEN ("developer tried to reconfigure the interval")
        {
            tick.SetInterval(10U);

            THEN ("a critical system error shall trigger")
            {
                REQUIRE (ASchMock::Assert::GetFails() == 1UL);
            }
        }
    }
}

SCENARIO ("Developer uses SysTick", "[SysTick]")
{
    Hal_Mock::InitSysTickRegisters();

    GIVEN ("SysTick is stopped")
    {
        Hal::SysTick tick = Hal::SysTick();
        tick.SetInterval(100U);

        WHEN ("SysTick is started")
        {
            tick.Start();

            THEN ("SysTick CSR bit 0 shall be set to 1")
            {
                REQUIRE (SYSTICK->CTRL == 0x1UL);
            }
            AND_WHEN ("running status is read")
            {
                bool isRunning = tick.IsRunning();

                THEN ("the read status shall be true")
                {
                    REQUIRE (isRunning == true);
                }
            }
        }
        AND_WHEN ("SysTick is started twice")
        {
            tick.Start();
            tick.Start();

            THEN ("SysTick CSR bit 0 shall be set to 1")
            {
                REQUIRE (SYSTICK->CTRL == 0x1UL);
            }
        }
    }

    GIVEN ("SysTick is running")
    {
        Hal::SysTick tick = Hal::SysTick();
        tick.SetInterval(100U);
        tick.Start();

        WHEN ("SysTick is stopped")
        {
            tick.Stop();

            THEN ("SysTick CSR bit 0 shall be set to 0")
            {
                REQUIRE (SYSTICK->CTRL == 0x0UL);
            }
            AND_WHEN ("running status is read")
            {
                bool isRunning = tick.IsRunning();

                THEN ("the read status shall be false")
                {
                    REQUIRE (isRunning == false);
                }
            }
        }
        AND_WHEN ("SysTick is stopped twice")
        {
            tick.Stop();
            tick.Stop();

            THEN ("SysTick CSR bit 0 shall be set to 0")
            {
                REQUIRE (SYSTICK->CTRL == 0x0UL);
            }
        }
    }
}
