

#ifndef SPINLOCK_H
#define SPINLOCK_H

typedef struct
{
    volatile int locked;

     

    int interrupts_enabled;

     

    unsigned int depth;

} spinlock_t;



void spinlock_init(
    spinlock_t *lock);



void spinlock_lock(
    spinlock_t *lock);

void spinlock_unlock(
    spinlock_t *lock);



int spinlock_is_locked(
    spinlock_t *lock);

#endif