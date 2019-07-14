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

//! @file    ASch_Main.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    13 May 2019
//!
//! @class   Main
//! @brief   !!!!! Brief file description here !!!!!
//! 
//! !!!!! Detailed file description here !!!!!

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>
#include <ASch_System.hpp>
#include <ASch_Scheduler.hpp>
#include <Hal_Gpio.hpp>

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

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

#if (UNIT_TEST == 1)
int main_test(void)
#else
int main(void)
#endif
{
    ASch::System::Init();    
    ASch::System::PreStartConfig();
    ASch::Scheduler::Start();
    ASch::Scheduler::MainLoop();
    return 0;
}
