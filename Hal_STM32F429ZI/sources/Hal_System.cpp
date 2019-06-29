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

//! @file    Hal_System.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    28 Oct 2018
//!
//! @class   System
//! @brief   This is HAL interface for system level functionality.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_System.hpp>
#include <stm32f4xx.h>
#include <Utils_Assert.hpp>
#include <Utils_Bit.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Static Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 6. Class Member Definitions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

System::System(void)
{
    return;
}

void System::Sleep(void)
{
    return;
}

void System::WakeUp(void)
{
    return;
}

void System::InitClocks(void)
{
    return;
}

void System::Reset(void)
{
    Utils::SetBit(SCB->AIRCR, 2UL, true);
    return;
}

void System::CriticalSystemError(void)
{
    return;
}

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------

