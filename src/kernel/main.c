#include "console.h"
#include "logger.h"
#include "stdio.h"

#include "timer.h"
#include "uart.h"

#include "trap.h"
#include "interrupt.h"
#include "plic.h"

#include "memory.h"

#include "task.h"
#include "scheduler.h"

#include "shell.h"

#include "panic.h"

void idle_task(void)
{
    while (1)
    {
        asm volatile("wfi");
    }
}

void shell_task(void)
{
    shell_init();
    shell_run();
    task_exit();
}

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
        "PLIC enabled");

     

    uart_enable_interrupts();

    klog_info(
        "UART interrupts enabled");

     

    memory_init();

    klog_info(
        "Memory initialized");

     

    task_init();

    scheduler_init();

    klog_info(
        "Scheduler initialized");

     

    kernel_ready = 1;

     

    task_create(
        "shell",
        shell_task);

    task_create(
        "idle",
        idle_task);

    klog_info(
        "Kernel tasks created");

     

    timer_boot_init();

    klog_info(
        "Timer configured");

    timer_enable();

    klog_info(
        "Timer enabled");

     

    klog_info(
        "Starting scheduler");

    scheduler_start();

    

    panic(
        "Scheduler returned");
}
