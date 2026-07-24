#include "queue.h"

#include "panic.h"

void queue_init(
    queue_t *queue)
{
    if (!queue)
        panic("queue_init NULL");

    list_init(
        &queue->head);

    queue->count = 0;
}

int queue_empty(
    queue_t *queue)
{
    if (!queue)
        panic("queue_empty NULL");

    return list_empty(
        &queue->head);
}

void queue_push(
    queue_t *queue,
    list_node_t *node)
{
    if (!queue || !node)
        panic("queue_push NULL");

    list_push_back(
        &queue->head,
        node);

    queue->count++;
}

list_node_t *queue_pop(
    queue_t *queue)
{
    if (!queue)
        panic("queue_pop NULL");

    list_node_t *node =
        list_front(
            &queue->head);

    if (!node)
        return 0;

    list_remove(
        node);

    queue->count--;

    return node;
}

list_node_t *queue_peek(
    queue_t *queue)
{
    if (!queue)
        panic("queue_peek NULL");

    return list_front(
        &queue->head);
}

size_t queue_size(
    queue_t *queue)
{
    if (!queue)
        panic("queue_size NULL");

    return queue->count;
}