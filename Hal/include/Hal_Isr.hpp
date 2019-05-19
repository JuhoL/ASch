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

#include <Utils_Types.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

//!
//! @brief   Different interrupt vectors available in the system.
//!
typedef enum
{
    interrupt_sysTick = 0,  //! SysTick vector.

    interrupt_global,
    interrupt_vectorsMax    //! The total number of vectors. Must be last on the enum!
} interruptType_t;

typedef void (*interruptHandler_t)(void);

} // namespace Hal

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

/// @class Isr
class Isr
{
public:
    explicit Isr(void);
    
    static_mf void SetHandler(interruptType_t type, interruptHandler_t Handler);
    static_mf void Enable(interruptType_t type);
    static_mf void Disable(interruptType_t type);

    // ToDo: Atomic ISR enable/disable for global interrupt control.

private:
    interruptHandler_t Handlers[interrupt_vectorsMax];
};

} // namespace Hal

#endif // HAL_ISR_HPP_
