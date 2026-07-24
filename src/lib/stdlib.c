#include "stdlib.h"

#include "errno.h"
#include "heap.h"
#include "memory.h"
#include "string.h"

void *malloc(size_t size)
{
    return heap_alloc(size);
}

void free(void *ptr)
{
    heap_free(ptr);
}

void *calloc(size_t count,
             size_t size)
{
    if (count == 0 ||
        size == 0)
    {
        errno = EINVAL;
        return 0;
    }

    size_t total =
        count * size;

    void *ptr =
        malloc(total);

    if (!ptr)
    {
        return 0;
    }

    kmemzero(
        ptr,
        total);

    errno = SUCCESS;

    return ptr;
}

void *realloc(void *ptr,
              size_t size)
{
    if (ptr == 0)
    {
        return malloc(size);
    }

    if (size == 0)
    {
        errno = EINVAL;

        free(ptr);

        return 0;
    }

    void *new_ptr =
        malloc(size);

    if (!new_ptr)
    {
        return 0;
    }

    size_t old_size =
        heap_block_size(ptr);

    size_t copy =
        (old_size < size)
            ? old_size
            : size;

    memcpy(
        new_ptr,
        ptr,
        copy);

    free(ptr);

    errno = SUCCESS;

    return new_ptr;
}

int atoi(
    const char *str)
{
    int sign = 1;

    int result = 0;

    if (*str == '-')
    {
        sign = -1;
        str++;
    }

    while (*str >= '0' &&
           *str <= '9')
    {
        result =
            result * 10 +
            (*str - '0');

        str++;
    }

    return result * sign;
}

long atol(
    const char *str)
{
    long sign = 1;

    long result = 0;

    if (*str == '-')
    {
        sign = -1;
        str++;
    }

    while (*str >= '0' &&
           *str <= '9')
    {
        result =
            result * 10 +
            (*str - '0');

        str++;
    }

    return result * sign;
}

int abs(
    int value)
{
    return (value < 0)
               ? -value
               : value;
}

void exit(
    int status)
{
    (void)status;

    while (1)
    {
        asm volatile("wfi");
    }
}
