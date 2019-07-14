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

//! @file    ASch_System.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 Aug 2018
//!
//! @class   System
//! @brief   Generic system control class for ASch.
//! 
//! This class implements system control functions and handles generic system level events like ticks and system errors. 

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#if (UNIT_TEST == 1)
    #define SYSTEM_UNIT_TEST    // For enabling test functions in ASch_TestConfiguration.hpp
#endif

#include <ASch_System.hpp>
#include <ASch_Scheduler.hpp>
#include <ASch_Configuration.hpp>
#include <Hal_System.hpp>

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

namespace ASch
{

//---------------------------------------
// Functions
//---------------------------------------

void System::Error(SysError error)
{
    return;
}

void System::Init(void)
{
    Scheduler::Init(Config::schedulerTickInterval);
    return;
}

void System::PreStartConfig(void)
{
    for (std::size_t i = 0; i < preStartConfigurationFunctionsMax; ++i)
    {
        apPreStartConfigFunctions[i]();
    }
    return;
}

} // namespace ASch

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------
