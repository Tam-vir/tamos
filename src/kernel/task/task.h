#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stddef.h>

#include "list.h"
#include "context.h"

#define MAX_TASKS 64

#define TASK_STACK_SIZE 4096

    typedef enum {

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

void task_exit(void);

#endif
