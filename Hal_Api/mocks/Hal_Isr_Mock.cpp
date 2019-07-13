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

//! @file    Hal_Isr_Mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 May 2019
//!
//! @brief   Mocks for HAL Isr.
//! 
//! These are mocks for HAL Isr utilising FakeIt.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Isr_Mock.hpp>
#include <Hal_Isr.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Initialisation
//-----------------------------------------------------------------------------------------------------------------------------

namespace HalMock
{

//! @class Isr
//! @brief This is a mock class for HAL Isr
class Isr
{
public:
    explicit Isr(void) {};
    virtual void Init(void);
    virtual void EnableGlobal(void);
    virtual void DisableGlobal(void);
    virtual void SetHandler(Interrupt type, interruptHandler_t Handler);
    virtual interruptHandler_t GetHandler(Interrupt type);
    virtual void Enable(Interrupt type);
    virtual void Disable(Interrupt type);
    virtual void SetPriority(Interrupt type, uint32_t priority);
    virtual uint8_t GetPriority(Interrupt type);
    virtual void SetPending(Interrupt type);
    virtual bool GetPending(Interrupt type);
    virtual void Clear(Interrupt type);
};

static Mock<Isr> mockHalIsr;
static HalMock::Isr& isr = mockHalIsr.get();

void InitSystem(void)
{
    static bool isFirstInit = true;

    if (isFirstInit == true)
    {
        Fake(Method(mockHalIsr, Init));
        Fake(Method(mockHalIsr, EnableGlobal));
        Fake(Method(mockHalIsr, DisableGlobal));
        Fake(Method(mockHalIsr, SetHandler));
        Fake(Method(mockHalIsr, GetHandler));
        Fake(Method(mockHalIsr, Enable));
        Fake(Method(mockHalIsr, Disable));
        Fake(Method(mockHalIsr, SetPriority));
        Fake(Method(mockHalIsr, GetPriority));
        Fake(Method(mockHalIsr, SetPending));
        Fake(Method(mockHalIsr, GetPending));
        Fake(Method(mockHalIsr, Clear));
    }
    else
    {
        mockHalIsr.Reset();
    }
    return;
}

} // namespace HalMock

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Mock Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

void Isr::Init(void)
{
    HalMock::isr.Init();
    return;
}

void Isr::EnableGlobal(void)
{
    HalMock::isr.EnableGlobal();
    return;
}

void Isr::DisableGlobal(void)
{
    HalMock::isr.DisableGlobal();
    return;
}

void Isr::SetHandler(Interrupt type, interruptHandler_t Handler)
{
    HalMock::isr.SetHandler(type, Handler);
    return;
}

interruptHandler_t Isr::GetHandler(Interrupt type)
{
    return HalMock::isr.GetHandler(type);
}

void Isr::Enable(Interrupt type)
{
    HalMock::isr.Enable(type);
    return;
}

void Isr::Disable(Interrupt type)
{
    HalMock::isr.Disable(type);
    return;
}

void Isr::SetPriority(Interrupt type, uint32_t priority)
{
    HalMock::isr.SetPriority(type, priority);
    return;
}

uint8_t Isr::GetPriority(Interrupt type)
{
    return HalMock::isr.GetPriority(type);
}

void Isr::SetPending(Interrupt type)
{
    HalMock::isr.SetPending(type);
    return;
}

bool Isr::GetPending(Interrupt type)
{
    HalMock::isr.GetPending(type);
    return;
}

void Isr::Clear(Interrupt type)
{
    HalMock::isr.Clear(type);
    return;
}


} // namespace Hal

