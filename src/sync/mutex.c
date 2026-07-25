#include "mutex.h"

#include "panic.h"
#include "task.h"

void mutex_init(
    mutex_t *mutex)
{
    if (!mutex)
    {
        panic(
            "mutex_init NULL");
    }

    mutex->locked = 0;

    spinlock_init(
        &mutex->lock);

    waitqueue_init(
        &mutex->wait_queue);
}

void mutex_lock(
    mutex_t *mutex)
{
    if (!mutex)
    {
        panic(
            "mutex_lock NULL");
    }

    spinlock_lock(
        &mutex->lock);

    while (mutex->locked)
    {
         

        task_block(
            &mutex->wait_queue,
            &mutex->lock);
    }

    mutex->locked = 1;

    spinlock_unlock(
        &mutex->lock);
}

void mutex_unlock(
    mutex_t *mutex)
{
    if (!mutex)
    {
        panic(
            "mutex_unlock NULL");
    }

    spinlock_lock(
        &mutex->lock);

    mutex->locked = 0;

     

    task_wakeup_queue(
        &mutex->wait_queue);

    spinlock_unlock(
        &mutex->lock);
}

int mutex_trylock(
    mutex_t *mutex)
{
    if (!mutex)
    {
        panic(
            "mutex_trylock NULL");
    }

    int success = 0;

    spinlock_lock(
        &mutex->lock);

    if (!mutex->locked)
    {
        mutex->locked = 1;

        success = 1;
    }

    spinlock_unlock(
        &mutex->lock);

    return success;
}

int mutex_is_locked(
    mutex_t *mutex)
{
    if (!mutex)
    {
        return 0;
    }

    return mutex->locked;
}