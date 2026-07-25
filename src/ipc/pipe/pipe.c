#include "pipe.h"

#include "task.h"
#include "panic.h"
#include "errno.h"

void pipe_init(
    pipe_t *pipe)
{
    if (!pipe)
    {
        panic(
            "pipe_init NULL");
    }

    ring_init(
        &pipe->buffer);

    spinlock_init(
        &pipe->lock);

    waitqueue_init(
        &pipe->not_empty);

    waitqueue_init(
        &pipe->not_full);

    pipe->closed = 0;
}

size_t pipe_write(
    pipe_t *pipe,
    const uint8_t *data,
    size_t len)
{
    if (!pipe || !data)
    {
        errno = EINVAL;
        return 0;
    }

    size_t written = 0;

    spinlock_lock(
        &pipe->lock);

    while (written < len)
    {
        
        if (pipe->closed)
        {
            break;
        }

        while (ring_full(&pipe->buffer) &&
               !pipe->closed)
        {
            task_block(
                &pipe->not_full,
                &pipe->lock);
        }

        if (pipe->closed)
        {
            break;
        }

        ring_push(
            &pipe->buffer,
            data[written]);

        written++;
    }

    if (written > 0)
    {
        task_wakeup_queue(
            &pipe->not_empty);
    }

    spinlock_unlock(
        &pipe->lock);

    errno = SUCCESS;

    return written;
}

size_t pipe_read(
    pipe_t *pipe,
    uint8_t *data,
    size_t len)
{
    if (!pipe || !data)
    {
        errno = EINVAL;
        return 0;
    }

    size_t got = 0;

    spinlock_lock(
        &pipe->lock);

    while (got < len)
    {
        while (ring_empty(&pipe->buffer) &&
               !pipe->closed)
        {
            task_block(
                &pipe->not_empty,
                &pipe->lock);
        }

        
        
        if (ring_empty(&pipe->buffer) &&
            pipe->closed)
        {
            break;
        }

        uint8_t byte;

        ring_pop(
            &pipe->buffer,
            &byte);

        data[got++] = byte;
    }

    if (got > 0)
    {
        task_wakeup_queue(
            &pipe->not_full);
    }

    spinlock_unlock(
        &pipe->lock);

    errno = SUCCESS;

    return got;
}

void pipe_close(
    pipe_t *pipe)
{
    if (!pipe)
    {
        return;
    }

    spinlock_lock(
        &pipe->lock);

    pipe->closed = 1;

    
    
    task_wakeup_queue(
        &pipe->not_empty);

    task_wakeup_queue(
        &pipe->not_full);

    spinlock_unlock(
        &pipe->lock);
}

int pipe_is_closed(
    pipe_t *pipe)
{
    if (!pipe)
    {
        return 1;
    }

    spinlock_lock(
        &pipe->lock);

    int closed =
        pipe->closed;

    spinlock_unlock(
        &pipe->lock);

    return closed;
}

size_t pipe_available(
    pipe_t *pipe)
{
    if (!pipe)
    {
        return 0;
    }

    spinlock_lock(
        &pipe->lock);

    size_t size =
        ring_size(&pipe->buffer);

    spinlock_unlock(
        &pipe->lock);

    return size;
}

size_t pipe_space(
    pipe_t *pipe)
{
    if (!pipe)
    {
        return 0;
    }

    spinlock_lock(
        &pipe->lock);

    size_t space =
        RING_SIZE -
        ring_size(&pipe->buffer);

    spinlock_unlock(
        &pipe->lock);

    return space;
}