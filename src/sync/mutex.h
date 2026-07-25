#ifndef MUTEX_H
#define MUTEX_H

#include <stdint.h>

#include "spinlock.h"
#include "waitqueue.h"

typedef struct
{
    int locked;

    spinlock_t lock;

    wait_queue_t wait_queue;

} mutex_t;

 

void mutex_init(
    mutex_t *mutex);

 

void mutex_lock(
    mutex_t *mutex);

 

void mutex_unlock(
    mutex_t *mutex);

 

int mutex_trylock(
    mutex_t *mutex);

 

int mutex_is_locked(
    mutex_t *mutex);

#endif