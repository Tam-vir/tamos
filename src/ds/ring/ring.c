#include "ring.h"

#include "panic.h"

void ring_init(
    ring_t *ring)
{
    if (!ring)
        panic("ring NULL");

    ring->read = 0;

    ring->write = 0;

    ring->count = 0;
}

int ring_empty(
    ring_t *ring)
{
    return ring->count == 0;
}

int ring_full(
    ring_t *ring)
{
    return ring->count == RING_SIZE;
}

int ring_push(
    ring_t *ring,
    uint8_t data)
{
    if (ring_full(ring))
        return -1;

    ring->buffer[ring->write] = data;

    ring->write++;

    if (ring->write >= RING_SIZE)
        ring->write = 0;

    ring->count++;

    return 0;
}

int ring_pop(
    ring_t *ring,
    uint8_t *data)
{
    if (ring_empty(ring))
        return -1;

    *data =
        ring->buffer[ring->read];

    ring->read++;

    if (ring->read >= RING_SIZE)
        ring->read = 0;

    ring->count--;

    return 0;
}

size_t ring_size(
    ring_t *ring)
{
    return ring->count;
}