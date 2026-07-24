#ifndef CLINT_H
#define CLINT_H

#include <stdint.h>

#define MTIMECMP_ADDR 0x2004000ULL
#define MTIME_ADDR 0x200BFF8ULL

static inline uint64_t
clint_read_mtime(void)
{
    volatile uint64_t *mtime =
        (volatile uint64_t *)MTIME_ADDR;

    return *mtime;
}

static inline void
clint_write_mtimecmp(uint64_t value)
{
    volatile uint64_t *mtimecmp =
        (volatile uint64_t *)MTIMECMP_ADDR;

    *mtimecmp = value;
}

#endif