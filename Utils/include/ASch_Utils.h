/******************************************************************************************************************************
 * Copyright (c) 2018 Juho Lepistö
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including without 
 * limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial
 * portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
 * TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *****************************************************************************************************************************/

/**
 * @file    ASch_Utils.h
 * @author  Juho Lepistö <juho.lepisto(a)gmail.com>
 * @date    16 Aug 2018
 * @brief   This is a collection of generic constants and utility inline functions.
 *
 * This is a collection of generic constants and utility inline functions that are used in ASch.
 */

#ifndef ASCH_UTILS_H_
#define ASCH_UTILS_H_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <cstdint>

namespace ASch
{

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Structs and Enums
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

template <typename type>
inline void IncrementIndexWithRollover(type &index, std::size_t maxLimit)
{
    if (++index >= maxLimit)
    {
        index = 0;
    }
    return;
}

template <typename type>
inline void DecrementIndexWithRollover(type &index, std::size_t maxLimit)
{
    if (index == 0)
    {
        index = static_cast<type>(maxLimit - 1);
    }
    else
    {
        index--;
    }
    return;
}

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//----------------------------------------------------------------------------------------------------------------------------


}

#endif // ASCH_UTILS_H_
