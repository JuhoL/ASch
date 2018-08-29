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

//! @file    ASch_Scheduler.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    22 Aug 2018
//!
//! @class   Scheduler
//! @brief   This is the scheduler module of ASch
//! 
//! The scheduler module is responsible of running tasks at given intervals based on system tick and run events reveiced
//! from the event module.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <ASch_Scheduler.hpp>

namespace ASch
{

namespace
{
//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Structs and Enums
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------


} // unnamed namespace
//-----------------------------------------------------------------------------------------------------------------------------
// 6. Class Member Definitions
//-----------------------------------------------------------------------------------------------------------------------------

Scheduler::Scheduler(Hal::SysTick& sysTickParameter, Hal::Isr& isrParameter, uint16_t tickIntervalInMs) : sysTick(sysTickParameter), isr(isrParameter)
{
    sysTick.SetInterval(tickIntervalInMs);
    isr.SetHandler(Hal::interrupt_sysTick, SysTickHandler);
    return;
}

uint8_t Scheduler::GetTaskCount(void) const
{
    return eventQueue.GetNumberOfElements();
}

void Scheduler::Start(void)
{
    isr.Enable(Hal::interrupt_sysTick);
    sysTick.Start();
    return;
}

void SysTickHandler(void)
{
    return;
}

} // namespace ASch
