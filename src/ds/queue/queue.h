#ifndef QUEUE_H
#define QUEUE_H

#include "list.h"
#include <stddef.h>

typedef struct
{
    list_node_t head;

    size_t count;

} queue_t;

void queue_init(
    queue_t *queue);

int queue_empty(
    queue_t *queue);

void queue_push(
    queue_t *queue,
    list_node_t *node);

list_node_t *queue_pop(
    queue_t *queue);

list_node_t *queue_peek(
    queue_t *queue);

size_t queue_size(
    queue_t *queue);

#endif