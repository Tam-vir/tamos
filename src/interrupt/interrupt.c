#include "interrupt.h"

#include "errno.h"
#include "panic.h"
#include "string.h"

static interrupt_handler_t
    interrupt_table[MAX_INTERRUPTS];

 

void interrupt_init(void)
{
    kmemzero(
        interrupt_table,
        sizeof(interrupt_table));

     

    asm volatile(
        "csrw mie, zero"
        :
        :
        : "memory");

     

    interrupt_disable();
}

 

void interrupt_enable(void)
{
    asm volatile(
        "csrs mstatus, %0"
        :
        : "r"(1UL << 3)
        : "memory");
}

 

void interrupt_disable(void)
{
    asm volatile(
        "csrc mstatus, %0"
        :
        : "r"(1UL << 3)
        : "memory");
}

int interrupt_is_enabled(void)
{
    uint64_t value;

    asm volatile(
        "csrr %0, mstatus"
        : "=r"(value));

    return (value & (1UL << 3))
               ? 1
               : 0;
}

 

void interrupt_enable_irq(
    uint64_t irq)
{
    switch (irq)
    {

    case IRQ_SOFTWARE:

        asm volatile(
            "csrs mie, %0"
            :
            : "r"(1UL << 3)
            : "memory");

        break;

    case IRQ_TIMER:

        asm volatile(
            "csrs mie, %0"
            :
            : "r"(1UL << 7)
            : "memory");

        break;

    case IRQ_EXTERNAL:

        asm volatile(
            "csrs mie, %0"
            :
            : "r"(1UL << 11)
            : "memory");

        break;

    default:

        break;
    }
}

 

void interrupt_disable_irq(
    uint64_t irq)
{
    switch (irq)
    {

    case IRQ_SOFTWARE:

        asm volatile(
            "csrc mie, %0"
            :
            : "r"(1UL << 3)
            : "memory");

        break;

    case IRQ_TIMER:

        asm volatile(
            "csrc mie, %0"
            :
            : "r"(1UL << 7)
            : "memory");

        break;

    case IRQ_EXTERNAL:

        asm volatile(
            "csrc mie, %0"
            :
            : "r"(1UL << 11)
            : "memory");

        break;

    default:

        break;
    }
}

 

int interrupt_irq_enabled(
    uint64_t irq)
{
    uint64_t mie;

    asm volatile(
        "csrr %0, mie"
        : "=r"(mie));

    switch (irq)
    {

    case IRQ_SOFTWARE:

        return (mie & (1UL << 3))
                   ? 1
                   : 0;

    case IRQ_TIMER:

        return (mie & (1UL << 7))
                   ? 1
                   : 0;

    case IRQ_EXTERNAL:

        return (mie & (1UL << 11))
                   ? 1
                   : 0;

    default:

        return 0;
    }
}

 

int interrupt_register(
    uint64_t irq,
    interrupt_handler_t handler)
{
    if (irq >= MAX_INTERRUPTS)
    {
        errno = EINVAL;
        return -1;
    }

    if (handler == 0)
    {
        errno = EINVAL;
        return -1;
    }

    if (interrupt_table[irq])
    {
        errno = EBUSY;
        return -1;
    }

    interrupt_table[irq] =
        handler;

    errno = SUCCESS;

    return 0;
}

 

int interrupt_unregister(
    uint64_t irq)
{
    if (irq >= MAX_INTERRUPTS)
    {
        errno = EINVAL;
        return -1;
    }

    if (!interrupt_table[irq])
    {
        errno = ENOENT;
        return -1;
    }

    interrupt_table[irq] = 0;

    errno = SUCCESS;

    return 0;
}

 

void interrupt_dispatch(
    uint64_t irq)
{
    if (irq >= MAX_INTERRUPTS)
    {
        panicf(
            "Invalid interrupt %u",
            irq);

        return;
    }

    if (interrupt_table[irq] == 0)
    {
        panicf(
            "Unhandled interrupt %u",
            irq);

        return;
    }

    interrupt_table[irq]();
}