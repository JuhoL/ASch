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

//! @file    ASch_Utils.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 Aug 2018
//!
//! @brief   This is a collection of generic constants and utility inline functions.
//! 
//! This is a collection of generic constants and utility inline functions that are used in ASch.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <ASch_Utils.hpp>

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

//-----------------------------------------------------------------------------------------------------------------------------
// 6. Class Member Definitions
//-----------------------------------------------------------------------------------------------------------------------------

/// @brief  This is blocker for new.
///
/// Got a linker error? Good! This function declaration is intentionally missing definition! See below for reasoning.
void* NewOperatorBlocker(void);

/// @brief  This is blocker for delete.
///
/// Got a linker error? Good! This function declaration is intentionally missing definition! See below for reasoning.
void DeleteOperatorBlocker(void);

/// @brief  This is overload for new operator
///
/// This function overloads new in C++ with a call of NewOperatorBlocker() which is not defined causing a linker error.
/// This will prevent hidden dynamic memory implementations, e.g. in C++ STL, by preventing linkage.
/// This devious trick is courtesy of Dan Saks.
void* operator new(std::size_t)
{
    return NewOperatorBlocker();
}

/// @brief  This is overload for delete operator
///
/// This function overloads new in C++ with a call of DeleteOperatorBlocker() which is not defined causing a linker error.
/// This is implemented only for sake of consistency. Intentionally breaking new is enough, but might just make sure it's dead.
void operator delete(void*)
{
    return DeleteOperatorBlocker();
}
