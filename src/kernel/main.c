#include "console.h"
#include "logger.h"
#include "stdio.h"
#include "timer.h"
#include "uart.h"
#include "trap.h"
#include "interrupt.h"
#include "plic.h"
#include "shell.h"
#include "memory.h"
#include "panic.h"

void kernel_main(void)
{

     
    console_init();

     
    logger_init();

    kprintf(
        "\n================================\n");

    kprintf(
        "       TamCore Kernel Boot\n");

    kprintf(
        "================================\n\n");

    klog_info(
        "Console initialized");

    klog_info(
        "Logger initialized");

     
    interrupt_init();

    klog_info(
        "Interrupt manager initialized");

     
    trap_init();

    klog_info(
        "Trap handler initialized");

     
    plic_init();

    klog_info(
        "PLIC initialized");

     

    plic_enable();

    klog_info(
        "PLIC interrupts enabled");

    uart_enable_interrupts();

    klog_info(
        "UART interrupts enabled");

     
    memory_init();

    klog_info(
        "Memory manager initialized");

     
    timer_boot_init();

    klog_info(
        "Timer configured");

     
    timer_enable();

    klog_info(
        "Timer interrupt enabled");

    

     
    kernel_ready = 1;

    klog_info(
        "Kernel ready");

     
    shell_init();

    klog_info(
        "Shell initialized");

    shell_run();

    panic(
        "Shell exited");
}