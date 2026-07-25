#include "semaphore.h"

#include "task.h"
#include "panic.h"

void semaphore_init(
    semaphore_t *sem,
    int value)
{
    if (!sem)
    {
        panic(
            "semaphore_init NULL");
    }

    sem->count =
        value;

    spinlock_init(
        &sem->lock);

    waitqueue_init(
        &sem->wait_queue);
}

void semaphore_wait(
    semaphore_t *sem)
{
    if (!sem)
    {
        panic(
            "semaphore_wait NULL");
    }

    spinlock_lock(
        &sem->lock);

    while (sem->count <= 0)
    {
         

        task_block(
            &sem->wait_queue,
            &sem->lock);
    }

    sem->count--;

    spinlock_unlock(
        &sem->lock);
}

void semaphore_post(
    semaphore_t *sem)
{
    if (!sem)
    {
        panic(
            "semaphore_post NULL");
    }

    spinlock_lock(
        &sem->lock);

    sem->count++;

     

    wait_entry_t *entry =
        waitqueue_wake_one(
            &sem->wait_queue);

    if (entry)
    {
        task_t *task =
            (task_t *)
                entry->data;

        task_unblock(
            task);
    }

    spinlock_unlock(
        &sem->lock);
}

int semaphore_try_wait(
    semaphore_t *sem)
{
    if (!sem)
    {
        return 0;
    }

    int success = 0;

    spinlock_lock(
        &sem->lock);

    if (sem->count > 0)
    {
        sem->count--;

        success = 1;
    }

    spinlock_unlock(
        &sem->lock);

    return success;
}

int semaphore_get_value(
    semaphore_t *sem)
{
    if (!sem)
    {
        return 0;
    }

    int value;

    spinlock_lock(
        &sem->lock);

    value =
        sem->count;

    spinlock_unlock(
        &sem->lock);

    return value;
}

void semaphore_destroy(
    semaphore_t *sem)
{
    if (!sem)
    {
        return;
    }

    spinlock_lock(
        &sem->lock);

    sem->count = 0;

     

    task_wakeup_queue(
        &sem->wait_queue);

    spinlock_unlock(
        &sem->lock);
}