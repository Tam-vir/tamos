#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>

#include "task.h"
#include "context.h"

    void scheduler_init(void);

void scheduler_add(
    task_t *task);

void scheduler_remove(
    task_t *task);

task_t *scheduler_next(void);

void scheduler_tick(
    cpu_context_t *ctx);

 
void scheduler_wakeup_tasks(void);

void scheduler_yield(void);

void scheduler_start(void);

task_t *scheduler_current(void);

#endif