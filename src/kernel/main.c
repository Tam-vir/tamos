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
#include "mutex.h"
#include "semaphore.h"
#include "cond.h"
#include "pipe.h"
#include "fd.h"
#include "unistd.h"
#include "string.h"

 

 
static mutex_t shared_mutex;
static semaphore_t data_sem;
static cond_t data_cond;
static pipe_t test_pipe;

 
static volatile uint64_t shared_counter = 0;
static volatile int cond_data_ready = 0;

 
static const char *
task_state_name(task_state_t state)
{
    switch (state)
    {
    case TASK_UNUSED:
        return "UNUSED";
    case TASK_READY:
        return "READY";
    case TASK_RUNNING:
        return "RUNNING";
    case TASK_BLOCKED:
        return "BLOCKED";
    case TASK_SLEEPING:
        return "SLEEPING";
    case TASK_ZOMBIE:
        return "ZOMBIE";
    default:
        return "?";
    }
}

static const char *
task_priority_name(uint8_t priority)
{
    switch (priority)
    {
    case TASK_PRIORITY_HIGH:
        return "HIGH";
    case TASK_PRIORITY_NORMAL:
        return "NORMAL";
    case TASK_PRIORITY_LOW:
        return "LOW";
    default:
        return "?";
    }
}

 
static void
scheduler_dump(void)
{
    task_t *tasks = task_table();
    uint64_t now = timer_get_ticks();

    kprintf("\n===================== SCHEDULER STATE (tick %u) =====================\n",
            (uint64_t)now);
    kprintf("PID  NAME       STATE     BASE    EFFECTIVE  WAITING\n");
    kprintf("----------------------------------------------------------------------\n");

    for (int i = 0; i < MAX_TASKS; i++)
    {
        task_t *task = &tasks[i];
        if (task->state == TASK_UNUSED)
            continue;

        kprintf("%u\t", (uint64_t)task->pid);
        kprintf("%s\t", task->name);
        kprintf("%s\t", task_state_name(task->state));
        kprintf("%s\t", task_priority_name(task->priority));
        kprintf("%s\t", task_priority_name(task->effective_priority));

        if (task->state == TASK_READY)
        {
            kprintf("%u ticks", (uint64_t)(now - task->ready_since_tick));
        }
        else
        {
            kprintf("-");
        }
        kprintf("\n");
    }

    kprintf("========================================================================\n\n");
}

void monitor_task(void)
{
    while (1)
    {
        scheduler_dump();
        task_sleep(30);  
    }
}

 
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

 
void producer_task(void)
{
    uint64_t count = 0;

    while (1)
    {
         
        const char *msg = "DATA";
        pipe_write(&test_pipe, (const uint8_t *)msg, 4);

         
        mutex_lock(&shared_mutex);
        shared_counter = ++count;
        cond_data_ready = 1;
        mutex_unlock(&shared_mutex);

         
        cond_signal(&data_cond);

         
        semaphore_post(&data_sem);

        kprintf("P");

         
        task_sleep(5);
    }
}

 
void consumer_task(void)
{
    uint8_t buf[32];

    while (1)
    {
         
        semaphore_wait(&data_sem);

         
        size_t n = pipe_read(&test_pipe, buf, sizeof(buf));
        if (n > 0)
        {
            kprintf("C");
        }

         
        mutex_lock(&shared_mutex);
        if (cond_data_ready)
        {
            kprintf("[c:%u]", (uint64_t)shared_counter);
            cond_data_ready = 0;
        }
        mutex_unlock(&shared_mutex);
    }
}

 
void mutex_task_a(void)
{
    while (1)
    {
        mutex_lock(&shared_mutex);

         
        for (volatile int i = 0; i < 50000; i++)
        {
        }

        mutex_unlock(&shared_mutex);

        kprintf("A");

         
        task_sleep(2);
    }
}

void mutex_task_b(void)
{
    while (1)
    {
        mutex_lock(&shared_mutex);

         
        for (volatile int i = 0; i < 50000; i++)
        {
        }

        mutex_unlock(&shared_mutex);

        kprintf("B");

        task_sleep(2);
    }
}

 
void cond_waiter_task(void)
{
    while (1)
    {
        mutex_lock(&shared_mutex);

         
        while (!cond_data_ready)
        {
            kprintf("w");  
            cond_wait(&data_cond, &shared_mutex);
        }

        kprintf("W[%u]", (uint64_t)shared_counter);
        cond_data_ready = 0;

        mutex_unlock(&shared_mutex);
    }
}

 
void cond_signaler_task(void)
{
    while (1)
    {
        task_sleep(15);  

        mutex_lock(&shared_mutex);
        cond_data_ready = 1;
        mutex_unlock(&shared_mutex);

        cond_broadcast(&data_cond);  
        kprintf("S");
    }
}

 
void syscall_user_task(void)
{
    int my_pid = getpid();
    kprintf("[sys_user pid=%d started]\n", my_pid);

     
    static pipe_t sys_pipe;
    pipe_init(&sys_pipe);

     
    int fd = fd_open_pipe(&sys_pipe);

    if (fd < 0)
    {
        kprintf("[sys_user: failed to open pipe fd!]");
        while (1)
        {
            sleep(100);
        }
    }

    uint64_t loop = 0;

    while (1)
    {
         
        const char *out_msg = "SYS";
        int64_t nw = write(fd, out_msg, 3);
        if (nw > 0)
        {
            kprintf("U");  
        }

         
        sleep(2);

         
        char buf[16];
        int64_t nr = read(fd, buf, sizeof(buf));
        if (nr > 0)
        {
            kprintf("u");  
        }

        sleep(3);

        loop++;

         
        if ((loop % 50) == 0)
        {
            kprintf("\n[sys_user: loop=%u, pid=%d]\n",
                    (uint64_t)loop, my_pid);
        }
    }
}

 
static mutex_t inv_mutex;

void inv_low_task(void)
{
    while (1)
    {
        mutex_lock(&inv_mutex);

        kprintf("l");  

         
        for (volatile uint64_t i = 0; i < 2000000; i++)
        {
        }

        mutex_unlock(&inv_mutex);
        kprintf("L");  

        task_sleep(5);
    }
}

void inv_med_task(void)
{
    while (1)
    {
         
        for (volatile uint64_t i = 0; i < 1500000; i++)
        {
        }
        kprintf("m");
        task_sleep(1);
    }
}

void inv_high_task(void)
{
    while (1)
    {
        kprintf("h");  

        mutex_lock(&inv_mutex);
        kprintf("H");  
        mutex_unlock(&inv_mutex);

        task_sleep(10);
    }
}

 
void kernel_main(void)
{
    console_init();
    logger_init();

    kprintf("\n================================\n");
    kprintf("   TamCore Feature Experiment\n");
    kprintf("================================\n\n");

    klog_info("Console initialized");
    klog_info("Logger initialized");

     
    interrupt_init();
    klog_info("Interrupt manager initialized");

     
    trap_init();
    klog_info("Trap handler initialized");

     
    plic_init();
    klog_info("PLIC initialized");
    plic_enable();
    klog_info("PLIC enabled");

     
    uart_enable_interrupts();
    klog_info("UART interrupts enabled");

     
    memory_init();
    klog_info("Memory initialized");

     
    mutex_init(&shared_mutex);
    mutex_init(&inv_mutex);
    semaphore_init(&data_sem, 0);
    cond_init(&data_cond);
    pipe_init(&test_pipe);
    klog_info("Synchronization primitives initialized");

     
    task_init();
    scheduler_init();
    klog_info("Scheduler initialized");

     
    fd_init();
    klog_info("File descriptor subsystem initialized");

     
    kernel_ready = 1;
    klog_info("Kernel ready flag set");

     

     
    task_create_priority("shell", shell_task, TASK_PRIORITY_HIGH);

     
    task_create_priority("producer", producer_task, TASK_PRIORITY_NORMAL);

     
    task_create_priority("consumer", consumer_task, TASK_PRIORITY_NORMAL);

     
    task_create_priority("mutex_a", mutex_task_a, TASK_PRIORITY_NORMAL);
    task_create_priority("mutex_b", mutex_task_b, TASK_PRIORITY_NORMAL);

     
    task_create_priority("cond_wtr", cond_waiter_task, TASK_PRIORITY_LOW);
    task_create_priority("cond_sig", cond_signaler_task, TASK_PRIORITY_NORMAL);

     
    task_create_priority("sys_user", syscall_user_task, TASK_PRIORITY_LOW);

     
    task_create_priority("inv_low", inv_low_task, TASK_PRIORITY_LOW);
    task_create_priority("inv_med", inv_med_task, TASK_PRIORITY_LOW);
    task_create_priority("inv_high", inv_high_task, TASK_PRIORITY_NORMAL);

     
    task_create_priority("monitor", monitor_task, TASK_PRIORITY_LOW);

     
    task_create_priority("idle", idle_task, TASK_PRIORITY_LOW);

    klog_info("All experiment tasks created");

     
    timer_boot_init();
    klog_info("Timer configured");
    timer_enable();
    klog_info("Timer enabled");

    klog_info("Starting scheduler...");

     
    scheduler_start();

     
    panic("Scheduler returned to kernel_main!");
}