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

/// @brief This function returns a single-bit bitmask of given bit position.
/// @param position - The bit position.
/// @return Single-bit bitmask.
inline uint32_t Bit(uint32_t position)
{
    return 1UL << position;
}

/// @brief This function returns the state of the given bit position from a bitfield.
/// @param bitfield - The bitfield ot be checked.
/// @param position - The bit position.
/// @return Bit state.
template <typename type>
inline bool GetBit(type bitfield, uint32_t position)
{
    return (bitfield & Bit(position)) != 0UL;
}

/// @brief This function returns a partial bitfield from the given position from a bitfield.
/// For example from bitfield 10100101b, position 3 with mask 111b results in 100b.
/// @param bitfield - The bitfield to be checked.
/// @param position - The partial bitfield position.
/// @param mask - The bitfield mask.
/// @return Partial bitfield
template <typename type>
inline uint32_t GetBits(type bitfield, uint32_t position, uint32_t mask)
{
    bitfield >>= position;
    bitfield &= mask;
    return bitfield;
}

/// @brief This function sets the state of the given bit position in a bitfield.
/// @param bitfield - A bitfield to be manipulated.
/// @param position - The bit position.
/// @param state - The new bit state.
/// @return The modified bitfield.
template <typename type>
inline void SetBit(type& bitfield, uint32_t position, bool state)
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

/// @brief This function sets a partial bitfield in the given position in a bitfield.
/// For example in a bitfield 10100101b, position 3 with mask 111b and pattern 011b results in 10011101b.
/// @param bitfield - A bitfield to be manipulated.
/// @param position - The partial bitfield position.
/// @param mask - The bitfield mask.
/// @param pattern - The bitfield pattern.
/// @return The modified bitfield.
template <typename type>
inline void SetBits(type& bitfield, uint32_t position, uint32_t mask, uint32_t pattern)
{
    mask <<= position;
    pattern <<= position;
    pattern &= mask;

    bitfield &= ~mask;
    bitfield |= pattern;
    return;
}

/// @brief This function compares given partial bitfield to another bitfield from the given position.
/// If the bitfields match, the function will return true.
/// For example from bitfield 10100101b, position 3 with mask 111b and pattern 100b results in true.
/// @param bitfield - The bitfield to be checked.
/// @param position - The partial bitfield position.
/// @param mask - The bitfield mask.
/// @param pattern - The pattern to compare.
/// @return True if the partial bitfield mathes
template <typename type>
inline bool CompareBits(type bitfield, uint32_t position, uint32_t mask, uint32_t pattern)
{
    bitfield >>= position;
    bitfield &= mask;
    pattern &= mask;
    return (bitfield == pattern);
}

} // namespace Utils

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

#endif // UTILS_BIT_HPP_
