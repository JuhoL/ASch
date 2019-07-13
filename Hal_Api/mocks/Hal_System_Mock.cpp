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

//! @file    Hal_System_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for HAL System.
//! 
//! These are mocks for HAL System utilising FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_System_Mock.hpp>
#include <Hal_System.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Initialisation
//-----------------------------------------------------------------------------------------------------------------------------

namespace HalMock
{

//! @class System
//! @brief This is HAL interface for system level functionality.
//! This class handler system clocks and power management.
class System
{
public:
    explicit System(void);
    virtual void Sleep(void);
    virtual void WakeUp(void);
    virtual void InitPowerControl(void);
    virtual void InitClocks(void);
    virtual void Reset(void);
    virtual void CriticalSystemError(void);
};

static Mock<System> mockHalSystem;

void InitSystem(void)
{
    static bool isFirstInit = true;

    if (isFirstInit == true)
    {
        Fake(Method(mockHalSystem, Sleep));
        Fake(Method(mockHalSystem, WakeUp));
        Fake(Method(mockHalSystem, InitPowerControl));
        Fake(Method(mockHalSystem, InitClocks));
        Fake(Method(mockHalSystem, Reset));
        Fake(Method(mockHalSystem, CriticalSystemError));
    }
    else
    {
        mockHalSystem.Reset();
    }
    return;
}

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

System::System(void)
{
    return;
}

void System::Sleep(void)
{
    // HalMock::System& system = mockHalSystem.get();
    // system.Sleep();
    return;
}

void System::WakeUp(void)
{
    // HalMock::System& system = mockHalSystem.get();
    // system.WakeUp();
    return;
}

void System::InitPowerControl(void)
{
    // HalMock::System& system = mockHalSystem.get();
    // system.InitPowerControl();
    return;
}

void System::InitClocks(void)
{
    // HalMock::System& system = mockHalSystem.get();
    // system.InitClocks();
    return;
}

void System::Reset(void)
{
    // HalMock::System& system = mockHalSystem.get();
    // system.Reset();
    return;
}

void System::CriticalSystemError(void)
{
    // HalMock::System& system = mockHalSystem.get();
    // system.CriticalSystemError();
    return;
}

} // namespace ASch

