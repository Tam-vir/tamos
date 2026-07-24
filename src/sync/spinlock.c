

#include "spinlock.h"

#include "interrupt.h"
#include "panic.h"



void spinlock_init(
    spinlock_t *lock)
{
    if (!lock)
    {
        panic(
            "spinlock_init(): NULL lock.");
    }

    lock->locked = 0;
    lock->interrupts_enabled = 1;
    lock->depth = 0;
}



void spinlock_lock(
    spinlock_t *lock)
{
    if (!lock)
    {
        panic(
            "spinlock_lock(): NULL lock.");
    }

     

    if (lock->locked)
    {
        lock->depth++;

        return;
    }

     

    lock->interrupts_enabled =
        interrupt_is_enabled();

     

    interrupt_disable();

    lock->locked = 1;
    lock->depth = 1;
}



void spinlock_unlock(
    spinlock_t *lock)
{
    if (!lock)
    {
        panic(
            "spinlock_unlock(): NULL lock.");
    }

    if (!lock->locked)
    {
        panic(
            "Attempted to unlock an unlocked spinlock.");
    }

     

    if (lock->depth > 1)
    {
        lock->depth--;

        return;
    }

    lock->depth = 0;
    lock->locked = 0;

     

    if (lock->interrupts_enabled)
    {
        interrupt_enable();
    }
}



int spinlock_is_locked(
    spinlock_t *lock)
{
    if (!lock)
    {
        return 0;
    }

    return lock->locked;
}