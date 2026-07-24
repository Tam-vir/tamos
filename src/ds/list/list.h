#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdint.h>

 

typedef struct list_node
{
    struct list_node *prev;

    struct list_node *next;

} list_node_t;

 

#define container_of(ptr, type, member) \
    ((type *)((uint8_t *)(ptr) -        \
              offsetof(type, member)))

 

void list_init(
    list_node_t *list);



int list_empty(
    list_node_t *list);



void list_insert_after(
    list_node_t *pos,
    list_node_t *node);

void list_insert_before(
    list_node_t *pos,
    list_node_t *node);

void list_push_front(
    list_node_t *list,
    list_node_t *node);

void list_push_back(
    list_node_t *list,
    list_node_t *node);



void list_remove(
    list_node_t *node);



list_node_t *list_front(
    list_node_t *list);

list_node_t *list_back(
    list_node_t *list);



size_t list_size(
    list_node_t *list);

#endif