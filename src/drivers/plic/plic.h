#ifndef PLIC_H
#define PLIC_H

#include <stdint.h>

#define PLIC_BASE 0x0C000000UL

#define PLIC_MAX_IRQ 1024

void plic_init(void);

 
void plic_enable(void);

void plic_disable(void);

 

void plic_set_priority(
    uint32_t irq,
    uint32_t priority);

 

void plic_enable_irq(
    uint32_t irq);

void plic_disable_irq(
    uint32_t irq);

 

uint32_t plic_claim(void);

void plic_complete(
    uint32_t irq);

#endif