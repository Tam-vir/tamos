.section .text
.align 2

.globl trap_entry


trap_entry:

    addi sp, sp, -272

    sd ra,   0(sp)


    sd t0,   8(sp)
    sd t1,  16(sp)
    sd t2,  24(sp)



    sd s0,  32(sp)
    sd s1,  40(sp)



    sd a0,  48(sp)
    sd a1,  56(sp)
    sd a2,  64(sp)
    sd a3,  72(sp)
    sd a4,  80(sp)
    sd a5,  88(sp)
    sd a6,  96(sp)
    sd a7, 104(sp)



    sd s2, 112(sp)
    sd s3, 120(sp)
    sd s4, 128(sp)
    sd s5, 136(sp)
    sd s6, 144(sp)
    sd s7, 152(sp)
    sd s8, 160(sp)
    sd s9, 168(sp)
    sd s10,176(sp)
    sd s11,184(sp)



    sd t3,192(sp)
    sd t4,200(sp)
    sd t5,208(sp)
    sd t6,216(sp)


    addi t0,sp,272

    sd t0,224(sp)



    sd gp,232(sp)

    sd tp,240(sp)


    csrr t0,mepc

    sd t0,248(sp)



    csrr t0,mcause

    sd t0,256(sp)



    csrr t0,mstatus

    sd t0,264(sp)

    mv a0,sp


    call trap_handler


    ld t0,264(sp)
    csrw mstatus,t0



    ld t0,248(sp)
    csrw mepc,t0




     

    ld ra,0(sp)

    # Stash this task's real t0 value in mscratch. We need t0 as
    # a scratch register further down to compute the (possibly
    # new) task's stack pointer, and we restore the real value
    # right before mret.
    ld t0,8(sp)
    csrw mscratch,t0

    ld t1,16(sp)
    ld t2,24(sp)



    ld s0,32(sp)
    ld s1,40(sp)



    ld a0,48(sp)
    ld a1,56(sp)
    ld a2,64(sp)
    ld a3,72(sp)
    ld a4,80(sp)
    ld a5,88(sp)
    ld a6,96(sp)
    ld a7,104(sp)



    ld s2,112(sp)
    ld s3,120(sp)
    ld s4,128(sp)
    ld s5,136(sp)
    ld s6,144(sp)
    ld s7,152(sp)
    ld s8,160(sp)
    ld s9,168(sp)
    ld s10,176(sp)
    ld s11,184(sp)



    ld t3,192(sp)
    ld t4,200(sp)
    ld t5,208(sp)
    ld t6,216(sp)


    ld gp,232(sp)

    ld tp,240(sp)

    # Switch to the current task's saved stack pointer (this is
    # the field scheduler_tick() updates when it picks a new
    # task). This is the actual context switch — everything
    # above just restored register *values*; this is what makes
    # us resume on the right stack.
    ld t0,224(sp)
    mv sp,t0

    # Now that sp has been switched, recover the real t0 value.
    csrr t0,mscratch

    mret