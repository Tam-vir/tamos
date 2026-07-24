#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stddef.h>

#include "list.h"
#include "context.h"
#include "waitqueue.h"
#include "spinlock.h"

#define MAX_TASKS 64
#define TASK_STACK_SIZE 4096

typedef enum
{
    TASK_UNUSED = 0,
    TASK_READY,
    TASK_RUNNING,
    TASK_BLOCKED,
    TASK_SLEEPING,
    TASK_ZOMBIE

} task_state_t;

typedef struct task
{
    list_node_t node;

    uint32_t pid;

    task_state_t state;

    uint64_t wake_tick;

    cpu_context_t context;

    uint8_t stack[TASK_STACK_SIZE];

    void (*entry)(void);

    char name[32];

    // Lets this task be linked into any wait_queue_t (uart rx,
    // future mutexes/pipes/etc.) without allocating anything.
    wait_entry_t wait_entry;

} task_t;

void task_init(void);

task_t *task_create(
    const char *name,
    void (*entry)(void));

task_t *task_get(
    uint32_t pid);

task_t *task_current(void);

void task_set_current(
    task_t *task);

task_t *task_table(void);

void task_sleep(
    uint64_t ticks);

void task_wakeup(
    task_t *task);

// Block the current task on `queue`. If `lock` is non-NULL, it is
// assumed to be held by the caller (protecting whatever condition
// was just checked) and is released only after the task is safely
// enqueued and marked TASK_BLOCKED, then re-acquired before this
// function returns to the caller. This closes the lost-wakeup race
// between "check condition" and "go to sleep".
void task_block(
    wait_queue_t *queue,
    spinlock_t *lock);

// Move a single TASK_BLOCKED task back onto the ready queue.
void task_unblock(
    task_t *task);

// Wake every task currently blocked on `queue`.
void task_wakeup_queue(
    wait_queue_t *queue);

void task_kill(
    task_t *task);

void task_destroy(
    task_t *task);

void task_wait(
    uint32_t pid);

void task_cleanup(void);

void task_exit(void);

#endif