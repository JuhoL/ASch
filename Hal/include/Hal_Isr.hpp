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

//! @file    Hal_Isr.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    29 Aug 2018
//!
//! @class   Isr
//! @brief   HAL interface for ISRs.
//! 
//! The ISR module manages interrupt vectors, global interrupt enable state, and NVIC configurations.

#ifndef HAL_ISR_HPP_
#define HAL_ISR_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <cstdint>

namespace Hal
{

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs and Constants
//-----------------------------------------------------------------------------------------------------------------------------


//-----------------------------------------------------------------------------------------------------------------------------
// 3. Structs and Enums
//-----------------------------------------------------------------------------------------------------------------------------

//!
//! @brief   Different interrupt vectors available in the system.
//!
typedef enum
{
    interrupt_sysTick = 0,  //! SysTick vector.

    interrupt_vectorsMax    //! The total number of vectors. Must be last on the enum!
} interruptType_t;

typedef void (*interruptHandler_t)(void);

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

/// @class Isr
class Isr
{
public:
    Isr(void);
    virtual void SetHandler(interruptType_t type, interruptHandler_t Handler);
    virtual void Enable(interruptType_t type);
    virtual void Disable(interruptType_t type);

private:
    interruptHandler_t Handlers[interrupt_vectorsMax];
};

} // namespace ASch

#endif // HAL_ISR_HPP_
