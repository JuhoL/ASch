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

//! @file    ASch_Configuration.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    28 Aug 2018
//!
//! @brief   System configuration header for ASch.
//! 
//! This class configures certain system parameters

#ifndef ASCH_CONFIGURATION_HPP_
#define ASCH_CONFIGURATION_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

typedef void (*configFunction_t)(void);

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Post Includes
//-----------------------------------------------------------------------------------------------------------------------------

#if (UNIT_TEST == 1)
    #include <ASch_TestConfiguration.hpp>
#else
    #include <ASch_ReleaseConfiguration.hpp>
#endif

namespace ASch
{

const std::size_t preStartConfigurationFunctionsMax = sizeof(apPreStartConfigFunctions)/sizeof(configFunction_t);

}

#endif // ASCH_CONFIGURATION_HPP_
