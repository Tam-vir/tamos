#ifndef WAITQUEUE_H
#define WAITQUEUE_H

#include "list.h"

#include <stddef.h>

 

typedef struct wait_entry
{
    list_node_t node;

    void *data;

} wait_entry_t;

 

typedef struct
{
    list_node_t head;

    size_t count;

} wait_queue_t;



void waitqueue_init(
    wait_queue_t *queue);



void waitqueue_add(
    wait_queue_t *queue,
    wait_entry_t *entry);



void waitqueue_remove(
    wait_queue_t *queue,
    wait_entry_t *entry);



wait_entry_t *waitqueue_wake_one(
    wait_queue_t *queue);



void waitqueue_wake_all(
    wait_queue_t *queue);



int waitqueue_empty(
    wait_queue_t *queue);

size_t waitqueue_size(
    wait_queue_t *queue);

#endif