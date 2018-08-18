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
 * @file    UTest_ASch_Queue.cpp
 * @author  Juho Lepistö <juho.lepisto(a)gmail.com>
 * @date    18 Aug 2018
 * @brief   This is a generic queue class.
 *
 * These are unit tests for ASch_Queue.cpp
 */

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <ASch_Queue.h>
#include <ASch_Queue.cpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

typedef struct
{
    uint32_t a;
    uint32_t b;
} testStruct_t;

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

TEST_CASE("Queue is created")
{
    ASch::Queue<testStruct_t, 5> queue = ASch::Queue<testStruct_t, 5>();

    REQUIRE (queue.GetNumberOfElements() == 0);
}
