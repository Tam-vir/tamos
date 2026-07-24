#include "list.h"

#include "panic.h"





void list_init(
    list_node_t *list)
{
    if (!list)
    {
        panic(
            "list_init: NULL list");
    }

    list->next = list;

    list->prev = list;
}



int list_empty(
    list_node_t *list)
{
    if (!list)
    {
        panic(
            "list_empty: NULL list");
    }

    return list->next == list;
}



void list_insert_after(
    list_node_t *pos,
    list_node_t *node)
{
    if (!pos || !node)
    {
        panic(
            "list_insert_after: NULL");
    }

    node->next =
        pos->next;

    node->prev =
        pos;

    pos->next->prev =
        node;

    pos->next =
        node;
}



void list_insert_before(
    list_node_t *pos,
    list_node_t *node)
{
    if (!pos || !node)
    {
        panic(
            "list_insert_before: NULL");
    }

    node->prev =
        pos->prev;

    node->next =
        pos;

    pos->prev->next =
        node;

    pos->prev =
        node;
}



void list_push_front(
    list_node_t *list,
    list_node_t *node)
{
    list_insert_after(
        list,
        node);
}



void list_push_back(
    list_node_t *list,
    list_node_t *node)
{
    list_insert_before(
        list,
        node);
}



void list_remove(
    list_node_t *node)
{
    if (!node)
    {
        panic(
            "list_remove: NULL");
    }

    node->prev->next =
        node->next;

    node->next->prev =
        node->prev;

     

    node->next =
        node;

    node->prev =
        node;
}



list_node_t *list_front(
    list_node_t *list)
{
    if (!list ||
        list_empty(list))
    {
        return 0;
    }

    return list->next;
}



list_node_t *list_back(
    list_node_t *list)
{
    if (!list ||
        list_empty(list))
    {
        return 0;
    }

    return list->prev;
}



size_t list_size(
    list_node_t *list)
{
    if (!list)
    {
        panic(
            "list_size: NULL");
    }

    size_t count = 0;

    list_node_t *current =
        list->next;

    while (current != list)
    {
        count++;

        current =
            current->next;
    }

    return count;
}