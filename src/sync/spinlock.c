#include "spinlock.h"

#include "interrupt.h"
#include "panic.h"

static inline uint64_t cpu_id(void)
{
     

    return 0;
}

void spinlock_init(
    spinlock_t *lock)
{
    if (!lock)
    {
        panic(
            "spinlock_init NULL");
    }

    lock->locked = 0;

    lock->owner = 0;

    lock->depth = 0;

    lock->interrupts_enabled = 1;
}

void spinlock_lock(
    spinlock_t *lock)
{
    if (!lock)
    {
        panic(
            "spinlock_lock NULL");
    }

    uint64_t id =
        cpu_id();

     

    if (lock->locked &&
        lock->owner == id)
    {
        lock->depth++;

        return;
    }

    int old_irq =
        interrupt_is_enabled();

    interrupt_disable();

     

    while (
        __sync_lock_test_and_set(
            &lock->locked,
            1))
    {
         
    }

    lock->owner =
        id;

    lock->depth =
        1;

    lock->interrupts_enabled =
        old_irq;
}

void spinlock_unlock(
    spinlock_t *lock)
{
    if (!lock)
    {
        panic(
            "spinlock_unlock NULL");
    }

    uint64_t id =
        cpu_id();

    if (!lock->locked ||
        lock->owner != id)
    {
        panic(
            "spinlock ownership error");
    }

     

    if (lock->depth > 1)
    {
        lock->depth--;

        return;
    }

    lock->depth = 0;

    lock->owner = 0;

     

    __sync_lock_release(
        &lock->locked);

    if (lock->interrupts_enabled)
    {
        interrupt_enable();
    }
}

int spinlock_is_locked(
    spinlock_t *lock)
{
    if (!lock)
        return 0;

    return lock->locked;
}