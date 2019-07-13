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

//! @file    ASch_System_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for ASch System.
//! 
//! These are mocks for ASch System utilising FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <ASch_System_Mock.hpp>
#include <ASch_System.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Initialisation
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASchMock
{

//! @class System
//! @brief This is a mock class for ASch System
class System
{
public:
    explicit System(void) {};
    virtual void Error(SysError error);
    virtual void Init(void);
    virtual void PreStartConfig(void);
};

static Mock<System> mockASchSystem;
static ASchMock::System& system = mockASchSystem.get();

void InitSystem(void)
{
    static bool isFirstInit = true;

    if (isFirstInit == true)
    {
        Fake(Method(mockASchSystem, Error));
        Fake(Method(mockASchSystem, Init));
        Fake(Method(mockASchSystem, PreStartConfig));
    }
    else
    {
        mockASchSystem.Reset();
    }
    return;
}

} // namespace ASchMock

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

void System::Error(SysError error)
{
    ASchMock::system.Error(error);
    return;
}
void System::Init(void)
{
    ASchMock::system.Init();
    return;
}
void System::PreStartConfig(void)
{
    ASchMock::system.PreStartConfig();
    return;
}

} // namespace Hal

