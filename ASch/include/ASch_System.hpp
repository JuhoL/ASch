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

//! @file    ASch_System.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    20 Aug 2018
//!
//! @class   System
//! @brief   Generic system control class for ASch.
//! 
//! This class implements system control functions and handles generic system level events like ticks and system errors. 

#ifndef ASCH_SYSTEM_HPP_
#define ASCH_SYSTEM_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>
#include <Hal_System.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

/// @brief This is a system error enum.
enum class SysError
{
    invalidParameters = 0,          //!< A function was called with invalid parameters.
    bufferOverflow,                 //!< A buffer has overflown.
    insufficientResources,          //!< System resources (e.g. task quota) has ran out.
    multipleSchedulerInstances,     //!< A second scheduler is initialised.
    assertFailure,                  //!< A debug assert has failed.
    unknownError                    //!< An unknown error. Should never occur.
};

}

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

namespace ASch
{

//! @class System
//! @brief Generic system control class for ASch.
//! This class implements system control functions and handles generic system level events like ticks and system errors. 
class System
{
public:
    explicit System(void) {};

    /// @brief A destructor to be used to deinit the scheduler.
    ~System(void);

    /// @brief This fuction raises a system error.
    /// @param error - Error type.
    static_mf void Error(SysError error);

    /// @brief This fuction initialises the system, e.f. clocks and other base peripherals.
    static_mf void Init(void);

    /// @brief This function runs pre-start configuration functions.
    static_mf void PreStartConfig(void);
};

} // namespace ASch

#endif // ASCH_SYSTEM_HPP_
