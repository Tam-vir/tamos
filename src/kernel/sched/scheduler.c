#include "scheduler.h"

#include "panic.h"
#include "context.h"

static list_node_t ready_queue;

static task_t *current = 0;

void scheduler_init(void)
{
    list_init(
        &ready_queue);

    current = 0;
}

void scheduler_add(
    task_t *task)
{

    if (!task)
    {
        panic(
            "scheduler_add NULL");
    }

    task->state =
        TASK_READY;

    list_push_back(
        &ready_queue,
        &task->node);
}

void scheduler_remove(
    task_t *task)
{
    if (!task)
        return;

    list_remove(
        &task->node);
}

task_t *
scheduler_next(void)
{

    list_node_t *node =
        list_front(
            &ready_queue);

    if (!node)
        return 0;

    list_remove(
        node);

    return container_of(
        node,
        task_t,
        node);
}

void scheduler_tick(
    cpu_context_t *ctx)
{

    if (!ctx)
        return;

    task_t *next =
        scheduler_next();

    if (!next)
    {
        return;
    }

     

    if (!current)
    {

        current =
            next;

        current->state =
            TASK_RUNNING;

        task_set_current(
            current);

         

        *ctx =
            current->context;

        return;
    }

     

    current->context =
        *ctx;

    if (current->state ==
        TASK_RUNNING)
    {
        current->state =
            TASK_READY;

        scheduler_add(
            current);
    }

     

    current =
        next;

    current->state =
        TASK_RUNNING;

    task_set_current(
        current);

     

    *ctx =
        current->context;
}

void scheduler_yield(void)
{
    asm volatile(
        "ecall");
}

void scheduler_start(void)
{

    task_t *next =
        scheduler_next();

    if (!next)
    {
        panic(
            "No runnable task");
    }

    current =
        next;

    current->state =
        TASK_RUNNING;

    task_set_current(
        current);

     

    asm volatile(
        "mv sp,%0\n"
        "jr %1"
        :
        : "r"(current->context.sp),
          "r"(current->context.mepc));
}

task_t *
scheduler_current(void)
{
    return current;
}