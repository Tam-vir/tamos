#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>

#include "spinlock.h"
#include "waitqueue.h"

typedef struct
{
    int count;

    spinlock_t lock;

    wait_queue_t wait_queue;

} semaphore_t;

void semaphore_init(
    semaphore_t *sem,
    int value);

void semaphore_wait(
    semaphore_t *sem);

void semaphore_post(
    semaphore_t *sem);

int semaphore_try_wait(
    semaphore_t *sem);

int semaphore_get_value(
    semaphore_t *sem);

void semaphore_destroy(
    semaphore_t *sem);

#endif