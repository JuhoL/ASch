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

//! @file    Utils_Bit.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    22 Jun 2019
//!
//! @class   Bit
//! @brief   !!!!! Brief file description here !!!!!
//! 
//! !!!!! Detailed file description here !!!!!

#ifndef UTILS_BIT_HPP_
#define UTILS_BIT_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Utils
{

inline uint32_t Bit(uint32_t position)
{
    return 1UL << position;
}

inline bool GetBit(uint32_t bitfield, uint32_t position)
{
    return (bitfield & Bit(position)) != 0UL;
}

inline uint32_t GetBits(uint32_t bitfield, uint32_t position, uint32_t mask)
{
    bitfield >>= position;
    bitfield &= mask;
    return bitfield;
}

inline void SetBit(uint32_t bitfield, uint32_t position, bool state)
{
    if (state == true)
    {
        bitfield |= Bit(position);
    }
    else
    {
        bitfield &= ~Bit(position);
    }
    return;
}

inline void SetBits(uint32_t bitfield, uint32_t position, uint32_t mask, uint32_t pattern)
{
    mask <<= position;
    pattern <<= position;
    pattern &= mask;

    bitfield &= ~mask;
    bitfield |= pattern;
    return;
}

} // namespace Utils

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

#endif // UTILS_BIT_HPP_
