#ifndef TRAP_H
#define TRAP_H

#include <stdint.h>

#define INTERRUPT_MSI 3
#define INTERRUPT_MTI 7
#define INTERRUPT_MEI 11

#define IS_INTERRUPT(cause) \
    ((cause) & (1ULL << 63))

#define GET_CAUSE(cause) \
    ((cause) & ~(1ULL << 63))

extern int kernel_ready;

typedef struct
{
    uint64_t ra;

    uint64_t t0;
    uint64_t t1;
    uint64_t t2;

    uint64_t s0;
    uint64_t s1;

    uint64_t a0;
    uint64_t a1;
    uint64_t a2;
    uint64_t a3;
    uint64_t a4;
    uint64_t a5;
    uint64_t a6;
    uint64_t a7;

    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;

    uint64_t t3;
    uint64_t t4;
    uint64_t t5;
    uint64_t t6;

    uint64_t sp;
    uint64_t gp;
    uint64_t tp;

    uint64_t mepc;
    uint64_t mcause;
    uint64_t mstatus;

} trap_context_t;

 

void trap_init(void);


void trap_handler(
    trap_context_t *ctx);

#endif