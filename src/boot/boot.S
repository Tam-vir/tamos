.section .text
.globl _start

.extern trap_entry
.extern timer_boot_init
.extern kernel_main

_start:

    #==================================
    # Initialize Stack Pointer
    #==================================

    la      sp, _stack_top


    #==================================
    # Clear .bss Section
    #==================================

    la      t0, _bss_start
    la      t1, _bss_end

    bge     t0, t1, bss_done

bss_loop:
    sd      zero, 0(t0)
    addi    t0, t0, 8
    blt     t0, t1, bss_loop

bss_done:


    #==================================
    # Initialize Trap Vector
    #==================================

    la      t0, trap_entry
    csrw    mtvec, t0


    #==================================
    # Initialize Timer Hardware
    #
    # This only programs MTIMECMP.
    # Interrupts remain disabled.
    #==================================

    call    timer_boot_init


    #==================================
    # Enter Kernel
    #==================================

    call    kernel_main


    #==================================
    # Kernel should never return.
    #==================================

halt:
    wfi
    j       halt



.section .bss
.align 16

_stack_bottom:
    .space 16384

_stack_top:


.extern _bss_start
.extern _bss_end