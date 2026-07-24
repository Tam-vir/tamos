#ifndef TRAP_H
#define TRAP_H

#include <stdint.h>

#include "context.h"

#define INTERRUPT_MSI 3
#define INTERRUPT_MTI 7
#define INTERRUPT_MEI 11

#define IS_INTERRUPT(cause) \
    ((cause) & (1ULL << 63))

#define GET_CAUSE(cause) \
    ((cause) & ~(1ULL << 63))

extern int kernel_ready;

typedef cpu_context_t trap_context_t;

void trap_init(void);

void trap_handler(
    trap_context_t *ctx);

#endif