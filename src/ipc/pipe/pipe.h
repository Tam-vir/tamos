#ifndef PIPE_H
#define PIPE_H

#include <stddef.h>
#include <stdint.h>

#include "ring.h"
#include "spinlock.h"
#include "waitqueue.h"

typedef struct
{
    ring_t buffer;

    spinlock_t lock;

    wait_queue_t not_empty;

    wait_queue_t not_full;

    int closed;

} pipe_t;

void pipe_init(
    pipe_t *pipe);



size_t pipe_write(
    pipe_t *pipe,
    const uint8_t *data,
    size_t len);



size_t pipe_read(
    pipe_t *pipe,
    uint8_t *data,
    size_t len);




void pipe_close(
    pipe_t *pipe);

int pipe_is_closed(
    pipe_t *pipe);

size_t pipe_available(
    pipe_t *pipe);

size_t pipe_space(
    pipe_t *pipe);

#endif