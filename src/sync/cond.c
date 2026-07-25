#include "cond.h"

#include "task.h"
#include "panic.h"

void cond_init(
    cond_t *cond)
{
    if (!cond)
    {
        panic(
            "cond_init NULL");
    }

    waitqueue_init(
        &cond->wait_queue);

    spinlock_init(
        &cond->lock);
}

void cond_wait(
    cond_t *cond,
    mutex_t *mutex)
{
    if (!cond || !mutex)
    {
        panic(
            "cond_wait NULL");
    }

    
    
    
    
    
    
    spinlock_lock(
        &cond->lock);

    mutex_unlock(
        mutex);

    
    
    
    task_block(
        &cond->wait_queue,
        &cond->lock);

    spinlock_unlock(
        &cond->lock);

    
    
    
    mutex_lock(
        mutex);
}

void cond_signal(
    cond_t *cond)
{
    if (!cond)
    {
        panic(
            "cond_signal NULL");
    }

    spinlock_lock(
        &cond->lock);

    wait_entry_t *entry =
        waitqueue_wake_one(
            &cond->wait_queue);

    if (entry)
    {
        task_unblock(
            (task_t *)entry->data);
    }

    spinlock_unlock(
        &cond->lock);
}

void cond_broadcast(
    cond_t *cond)
{
    if (!cond)
    {
        panic(
            "cond_broadcast NULL");
    }

    spinlock_lock(
        &cond->lock);

    task_wakeup_queue(
        &cond->wait_queue);

    spinlock_unlock(
        &cond->lock);
}