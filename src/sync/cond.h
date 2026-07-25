#ifndef COND_H
#define COND_H

#include "spinlock.h"
#include "waitqueue.h"
#include "mutex.h"

typedef struct
{
    wait_queue_t wait_queue;

    spinlock_t lock;

} cond_t;

void cond_init(
    cond_t *cond);




void cond_wait(
    cond_t *cond,
    mutex_t *mutex);


void cond_signal(
    cond_t *cond);


void cond_broadcast(
    cond_t *cond);

#endif