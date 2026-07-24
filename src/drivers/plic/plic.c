#include "plic.h"

#define PLIC_PRIORITY_BASE 0x000000UL
#define PLIC_PENDING_BASE 0x001000UL
#define PLIC_ENABLE_BASE 0x002000UL

 

#define PLIC_CONTEXT_BASE 0x200000UL
#define PLIC_CONTEXT 0

static inline volatile uint32_t *
plic_reg(
    uint64_t offset)
{
    return (volatile uint32_t *)(PLIC_BASE + offset);
}

 

void plic_init(void)
{

     

    for (uint32_t i = 0;
         i < PLIC_MAX_IRQ / 32;
         i++)
    {

        volatile uint32_t *enable =
            plic_reg(
                PLIC_ENABLE_BASE +
                i * 4);

        *enable = 0;
    }

     

    for (uint32_t i = 0;
         i < PLIC_MAX_IRQ;
         i++)
    {

        volatile uint32_t *priority =
            plic_reg(
                PLIC_PRIORITY_BASE +
                i * 4);

        *priority = 0;
    }

     

    volatile uint32_t *threshold =
        plic_reg(
            PLIC_CONTEXT_BASE +
            PLIC_CONTEXT * 0x1000);

    *threshold = 0;
}

 

void plic_enable(void)
{

    uint64_t value =
        (1 << 11);

     

    asm volatile(
        "csrs mie,%0"
        :
        : "r"(value)
        : "memory");

     

    asm volatile(
        "csrs mstatus,%0"
        :
        : "r"(0x8)
        : "memory");
}

void plic_disable(void)
{

    asm volatile(
        "csrc mie,%0"
        :
        : "r"(1 << 11)
        : "memory");
}

 

void plic_set_priority(
    uint32_t irq,
    uint32_t priority)
{

    if (irq >= PLIC_MAX_IRQ)
        return;

    volatile uint32_t *reg =
        plic_reg(
            PLIC_PRIORITY_BASE +
            irq * 4);

    *reg = priority;
}

 

void plic_enable_irq(
    uint32_t irq)
{

    if (irq >= PLIC_MAX_IRQ)
        return;

    uint32_t word =
        irq / 32;

    uint32_t bit =
        irq % 32;

    volatile uint32_t *enable =
        plic_reg(
            PLIC_ENABLE_BASE +
            word * 4);

    *enable |=
        (1U << bit);
}

 

void plic_disable_irq(
    uint32_t irq)
{

    if (irq >= PLIC_MAX_IRQ)
        return;

    uint32_t word =
        irq / 32;

    uint32_t bit =
        irq % 32;

    volatile uint32_t *enable =
        plic_reg(
            PLIC_ENABLE_BASE +
            word * 4);

    *enable &=
        ~(1U << bit);
}

 

uint32_t plic_claim(void)
{

    volatile uint32_t *claim =
        plic_reg(
            PLIC_CONTEXT_BASE +
            PLIC_CONTEXT * 0x1000 +
            4);

    return *claim;
}

 

void plic_complete(
    uint32_t irq)
{

    volatile uint32_t *complete =
        plic_reg(
            PLIC_CONTEXT_BASE +
            PLIC_CONTEXT * 0x1000 +
            4);

    *complete = irq;
}