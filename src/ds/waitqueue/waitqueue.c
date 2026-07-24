#include "waitqueue.h"

#include "panic.h"

void waitqueue_init(
    wait_queue_t *queue)
{
    if (!queue)
    {
        panic(
            "waitqueue_init: NULL");
    }

    list_init(
        &queue->head);

    queue->count = 0;
}

void waitqueue_add(
    wait_queue_t *queue,
    wait_entry_t *entry)
{
    if (!queue || !entry)
    {
        panic(
            "waitqueue_add: NULL");
    }

    list_push_back(
        &queue->head,
        &entry->node);

    queue->count++;
}

void waitqueue_remove(
    wait_queue_t *queue,
    wait_entry_t *entry)
{
    if (!queue || !entry)
    {
        panic(
            "waitqueue_remove: NULL");
    }

    list_remove(
        &entry->node);

    if (queue->count)
    {
        queue->count--;
    }
}

wait_entry_t *waitqueue_wake_one(
    wait_queue_t *queue)
{
    if (!queue)
    {
        panic(
            "waitqueue_wake_one: NULL");
    }

    list_node_t *node =
        list_front(
            &queue->head);

    if (!node)
    {
        return 0;
    }

    list_remove(
        node);

    queue->count--;

    return container_of(
        node,
        wait_entry_t,
        node);
}

void waitqueue_wake_all(
    wait_queue_t *queue)
{
    if (!queue)
    {
        panic(
            "waitqueue_wake_all: NULL");
    }

    while (!list_empty(
        &queue->head))
    {
        waitqueue_wake_one(
            queue);
    }
}

int waitqueue_empty(
    wait_queue_t *queue)
{
    if (!queue)
    {
        panic(
            "waitqueue_empty: NULL");
    }

    return list_empty(
        &queue->head);
}

size_t waitqueue_size(
    wait_queue_t *queue)
{
    if (!queue)
    {
        panic(
            "waitqueue_size: NULL");
    }

    return queue->count;
}