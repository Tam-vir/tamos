#ifndef RING_H
#define RING_H

#include <stdint.h>
#include <stddef.h>

#define RING_SIZE 256

typedef struct
{
    uint8_t buffer[RING_SIZE];

    size_t read;

    size_t write;

    size_t count;

} ring_t;

void ring_init(
    ring_t *ring);

int ring_empty(
    ring_t *ring);

int ring_full(
    ring_t *ring);

int ring_push(
    ring_t *ring,
    uint8_t data);

int ring_pop(
    ring_t *ring,
    uint8_t *data);

size_t ring_size(
    ring_t *ring);

#endif