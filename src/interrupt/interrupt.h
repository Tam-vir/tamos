#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdint.h>

 

#define MAX_INTERRUPTS 256

 

#define IRQ_SOFTWARE 3
#define IRQ_TIMER 7
#define IRQ_EXTERNAL 11

typedef void (*interrupt_handler_t)(void);

 

void interrupt_init(void);

 

void interrupt_enable(void);

void interrupt_disable(void);

int interrupt_is_enabled(void);

 

void interrupt_enable_irq(
    uint64_t irq);

void interrupt_disable_irq(
    uint64_t irq);

int interrupt_irq_enabled(
    uint64_t irq);

 

int interrupt_register(
    uint64_t irq,
    interrupt_handler_t handler);

int interrupt_unregister(
    uint64_t irq);

 

void interrupt_dispatch(
    uint64_t irq);

#endif