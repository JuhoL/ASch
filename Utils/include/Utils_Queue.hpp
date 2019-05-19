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

//! @file    Utils_Queue.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    16 Aug 2018
//!
//! @class   Queue
//! @brief   This is a generic queue class.
//! 
//! This class implements a simple general purpose ring-buffer type queue that operates in FIFO method.

#ifndef UTILS_QUEUE_HPP_
#define UTILS_QUEUE_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>
#include <Utils_Misc.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

namespace Utils
{

template <typename ElementType, std::size_t size>
class Queue
{
public:
    Queue(void);

    bool Push(ElementType element);
    bool Pop(ElementType& element);
    
    uint8_t GetNumberOfElements(void) const;
    void Flush(void);

private:
    ElementType elements[size];
    std::size_t queueSize = size;

    uint8_t numberOfElements;
    uint8_t nextFreeIndex;
    uint8_t nextIndexInQueue;
};

template <typename ElementType, std::size_t size>
Queue<ElementType, size>::Queue(void)
{
    this->Flush();

    return;
}

template <typename ElementType, std::size_t size>
bool Queue<ElementType, size>::Push(ElementType element)
{
    bool errors;

    if (numberOfElements < queueSize)
    {
        elements[nextFreeIndex] = element;
        numberOfElements++;
        IncrementIndexWithRollover(nextFreeIndex, queueSize);

        errors = false;
    }
    else
    {
        errors = true;
    }
    
    return errors;
}

template <typename ElementType, std::size_t size>
bool Queue<ElementType, size>::Pop(ElementType& element)
{
    bool errors;

    if (numberOfElements > 0U)
    {
        element = elements[nextIndexInQueue];
        IncrementIndexWithRollover(nextIndexInQueue, queueSize);
        numberOfElements--;

        errors = false;
    }
    else
    {
        errors = true;
    }

    return errors;
}

template <typename ElementType, std::size_t size>
uint8_t Queue<ElementType, size>::GetNumberOfElements(void) const
{
    return numberOfElements;
}

template <typename ElementType, std::size_t size>
void Queue<ElementType, size>::Flush(void)
{
    numberOfElements = 0U;
    nextFreeIndex = 0U;
    nextIndexInQueue = 0U;

    return;
}

} // namespace Utils

#endif // UTILS_QUEUE_HPP_