#include <stdint.h>
#include "trap.h"
#include "interrupt.h"
#include "panic.h"
#include "plic.h"
#include "scheduler.h"
#include "stdio.h"

int kernel_ready = 0;

void trap_handler(trap_context_t *ctx)
{
    uint64_t cause = ctx->mcause;

    if (IS_INTERRUPT(cause))
    {
        uint64_t irq = GET_CAUSE(cause);

        switch (irq)
        {
        case INTERRUPT_MTI:
        {
            
            interrupt_dispatch(INTERRUPT_MTI);

            
            if (kernel_ready)
            {
                scheduler_tick(ctx);
            }
            break;
        }

        case INTERRUPT_MEI:
        {
            uint32_t device_irq = plic_claim();
            if (device_irq)
            {
                interrupt_dispatch(device_irq);
                plic_complete(device_irq);
            }
            break;
        }

        case INTERRUPT_MSI:
        {
            interrupt_dispatch(INTERRUPT_MSI);
            break;
        }

        default:
            panicf("Unknown interrupt %u", irq);
            break;
        }
        return;
    }

    
    switch (cause)
    {
    case 0:
        panic("Instruction address misaligned");
        break;
    case 1:
        panic("Instruction access fault");
        break;
    case 2:
        panic("Illegal instruction");
        break;
    case 3:
        panic("Breakpoint");
        break;
    case 4:
        panic("Load address misaligned");
        break;
    case 5:
        panic("Load access fault");
        break;
    case 6:
        panic("Store address misaligned");
        break;
    case 7:
        panic("Store access fault");
        break;
    case 8:
        panic("ECALL from U-mode");
        break;
    case 9:
        panic("ECALL from S-mode");
        break;
    case 11:            
        ctx->mepc += 4; 
        scheduler_tick(ctx);
        break;
    default:
        panicf("Unhandled exception %u", cause);
        break;
    }
}

void trap_init(void)
{
    extern void trap_entry(void);
    uint64_t addr = (uint64_t)&trap_entry;
    addr &= ~0x3ULL; 
    asm volatile("csrw mtvec, %0" : : "r"(addr) : "memory");
}