#include <Asch_Queue.h>
#include <ASch_Queue.cpp>

#include <cstdint>
#include <stdio.h>

typedef struct
{
    uint32_t a;
    uint32_t b;
} testStruct_t;

int main(void)
{
    testStruct_t test;
    bool errors;

    ASch::Queue<testStruct_t, 5> queue = ASch::Queue<testStruct_t, 5>();

    test.a = 1;
    test.b = 2;
    queue.Push(test);

    test.a = 3;
    test.b = 4;
    queue.Push(test);

    printf("Elements: %u\n", queue.GetNumberOfElements());
    printf("\n");

    errors = queue.Pop(test);
    printf("a = %u, b = %u\n", test.a, test.b);
    printf("Elements: %u\n", queue.GetNumberOfElements());
    printf("Errors: %u\n", errors);
    printf("\n");

    errors = queue.Pop(test);
    printf("a = %u, b = %u\n", test.a, test.b);
    printf("Elements: %u\n", queue.GetNumberOfElements());
    printf("Errors: %u\n", errors);
    printf("\n");

    errors = queue.Pop(test);
    printf("a = %u, b = %u\n", test.a, test.b);
    printf("Elements: %u\n", queue.GetNumberOfElements());
    printf("Errors: %u\n", errors);
    printf("\n");

    return 0;
}
