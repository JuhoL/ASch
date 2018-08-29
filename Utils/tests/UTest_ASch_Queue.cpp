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

//! @file    UTest_ASch_Queue.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    18 Aug 2018
//! 
//! @brief   This is a generic queue class.
//! 
//! These are unit tests for ASch_Queue.cpp

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#include <fakeit.hpp>

#include <ASch_Queue.hpp>
#include <ASch_Queue.cpp>

#include <typeinfo>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Structs and Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

typedef struct
{
    uint32_t number;
    char character;
} testStruct_t;

}

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Test Cases
//-----------------------------------------------------------------------------------------------------------------------------

SCENARIO ("Developer successfully uses queue", "[queue]")
{
    GIVEN ("the queue does not exist")
    {
        WHEN ("the developer creates a new queue")
        {
            ASch::Queue<testStruct_t, 3> queue = ASch::Queue<testStruct_t, 3>();

            THEN ("the new queue shall be empty")
            {
                REQUIRE (queue.GetNumberOfElements() == 0);
            }
        }
        AND_WHEN ("the developer creates another type of queue")
        {
            ASch::Queue<uint32_t, 5> queue = ASch::Queue<uint32_t, 5>();

            THEN ("it shall succeed and the new queue shall be empty")
            {
                REQUIRE (queue.GetNumberOfElements() == 0);
            }
        }
    }

    GIVEN ("the queue is created and empty")
    {
        ASch::Queue<testStruct_t, 3> queue = ASch::Queue<testStruct_t, 3>();
        WHEN ("the developer pushes an element into the queue")
        {
            testStruct_t testData = {.number = 1UL, .character = 'A'};
            bool errors = queue.Push(testData);

            THEN ("there shall be no errors")
            {
                REQUIRE (errors == false);
            }
            AND_THEN ("the queue shall have one element in it")
            {
                REQUIRE (queue.GetNumberOfElements() == 1);
            }
        }
        AND_WHEN ("three elements are pushed into the queue")
        {
            testStruct_t testData = {.number = 1UL, .character = 'A'};
            bool errors = queue.Push(testData);

            testData.number = 2UL;
            testData.character = 'B';
            errors |= queue.Push(testData);

            testData.number = 3UL;
            testData.character = 'C';
            errors |= queue.Push(testData);

            THEN ("there shall be no errors")
            {
                REQUIRE (errors == false);
            }
            AND_THEN ("the queue shall have three elements in it")
            {
                REQUIRE (queue.GetNumberOfElements() == 3);
            }
        }
    }

    GIVEN ("the queue is created and full")
    {
        ASch::Queue<testStruct_t, 3> queue = ASch::Queue<testStruct_t, 3>();
        testStruct_t testData = {.number = 1UL, .character = 'J'};
        bool errors = queue.Push(testData);

        testData.number = 2UL;
        testData.character = 'A';
        errors = queue.Push(testData);

        testData.number = 3UL;
        testData.character = 'S';
        errors = queue.Push(testData);

        WHEN ("the developer pops an element")
        {
            testData.number = 0UL;
            testData.character = '\0';
            errors = queue.Pop(testData);

            THEN ("there shall be no errors")
            {
                REQUIRE (errors == false);
            }
            AND_THEN ("the element shall be the first one pushed")
            {
                REQUIRE (testData.number == 1UL);
                REQUIRE (testData.character == 'J');
            }
            AND_THEN ("the queue shall have two elements in it")
            {
                REQUIRE (queue.GetNumberOfElements() == 2);
            }
            AND_WHEN ("the developer pops second element")
            {
                errors = queue.Pop(testData);

                THEN ("there shall be no errors")
                {
                    REQUIRE (errors == false);
                }
                AND_THEN ("the element shall be the second one pushed")
                {
                    REQUIRE (testData.number == 2UL);
                    REQUIRE (testData.character == 'A');
                }
                AND_THEN ("the queue shall have one element in it")
                {
                    REQUIRE (queue.GetNumberOfElements() == 1);
                }
                AND_WHEN ("the developer pops third element")
                {
                    errors = queue.Pop(testData);

                    THEN ("there shall be no errors")
                    {
                        REQUIRE (errors == false);
                    }
                    AND_THEN ("the element shall be the third one pushed")
                    {
                        REQUIRE (testData.number == 3UL);
                        REQUIRE (testData.character == 'S');
                    }
                    AND_THEN ("the queue shall be empty")
                    {
                        REQUIRE (queue.GetNumberOfElements() == 0);
                    }
                }
            }
        }

        WHEN ("the developer pops an element and pushes another one")
        {
            testData.number = 0UL;
            testData.character = '\0';
            errors = queue.Pop(testData);

            testData.number = 4UL;
            testData.character = 'L';
            errors = queue.Push(testData);

            THEN ("there shall be no errors")
            {
                REQUIRE (errors == false);
            }
            AND_THEN ("the queue shall have three elements in it")
            {
                REQUIRE (queue.GetNumberOfElements() == 3);
            }
        }

        WHEN ("the developer flushes the queue")
        {
            queue.Flush();

            THEN ("the queue shall be empty")
            {
                REQUIRE (queue.GetNumberOfElements() == 0);
            }
        }
    }
}

SCENARIO ("Developer misuses queue", "[queue]")
{
    GIVEN ("The queue is created and full")
    {
        ASch::Queue<testStruct_t, 3> queue = ASch::Queue<testStruct_t, 3>();
        testStruct_t testData = {.number = 1UL, .character = 'J'};
        bool errors = queue.Push(testData);

        testData.number = 2UL;
        testData.character = 'A';
        errors = queue.Push(testData);

        testData.number = 3UL;
        testData.character = 'S';
        errors = queue.Push(testData);

        WHEN ("the developer pushes an element into the queue")
        {
            testData = {.number = 4UL, .character = 'L'};
            errors = queue.Push(testData);

            THEN ("there shall be error")
            {
                REQUIRE (errors == true);
            }
            AND_THEN ("the queue shall still have three element in it")
            {
                REQUIRE (queue.GetNumberOfElements() == 3);
            }
        }
    }

    GIVEN ("the queue is created and empty")
    {
        ASch::Queue<testStruct_t, 3> queue = ASch::Queue<testStruct_t, 3>();
        WHEN ("the developer pops an element from the queue")
        {
            testStruct_t testData = {.number = 0UL, .character = '\0'};
            bool errors = queue.Pop(testData);

            THEN ("there shall be error")
            {
                REQUIRE (errors == true);
            }
            AND_THEN ("the queue shall still have zero elements in it")
            {
                REQUIRE (queue.GetNumberOfElements() == 0);
            }
        }
    }
}
