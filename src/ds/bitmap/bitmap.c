#include "bitmap.h"

#include "panic.h"
#include "string.h"



void bitmap_init(
    bitmap_t *map,
    uint8_t *buffer,
    size_t bits)
{
    if (!map || !buffer)
    {
        panic(
            "bitmap_init: NULL argument");
    }

    map->bits =
        buffer;

    map->size =
        bits;

     

    kmemzero(
        buffer,
        (bits + 7) / 8);
}



void bitmap_set(
    bitmap_t *map,
    size_t index)
{
    if (!map)
    {
        panic(
            "bitmap_set: NULL bitmap");
    }

    if (index >= map->size)
    {
        panic(
            "bitmap_set: out of range");
    }

    size_t byte =
        index / 8;

    size_t bit =
        index % 8;

    map->bits[byte] |=
        (1 << bit);
}



void bitmap_clear(
    bitmap_t *map,
    size_t index)
{
    if (!map)
    {
        panic(
            "bitmap_clear: NULL bitmap");
    }

    if (index >= map->size)
    {
        panic(
            "bitmap_clear: out of range");
    }

    size_t byte =
        index / 8;

    size_t bit =
        index % 8;

    map->bits[byte] &=
        ~(1 << bit);
}



int bitmap_test(
    bitmap_t *map,
    size_t index)
{
    if (!map)
    {
        panic(
            "bitmap_test: NULL bitmap");
    }

    if (index >= map->size)
    {
        panic(
            "bitmap_test: out of range");
    }

    size_t byte =
        index / 8;

    size_t bit =
        index % 8;

    return (map->bits[byte] &
            (1 << bit))
               ? 1
               : 0;
}



int bitmap_find_free(
    bitmap_t *map)
{
    if (!map)
    {
        panic(
            "bitmap_find_free: NULL bitmap");
    }

    for (size_t i = 0;
         i < map->size;
         i++)
    {
        if (!bitmap_test(
                map,
                i))
        {
            return (int)i;
        }
    }

    return -1;
}



int bitmap_find_free_range(
    bitmap_t *map,
    size_t count)
{
    if (!map)
    {
        panic(
            "bitmap_find_free_range: NULL bitmap");
    }

    if (count == 0)
    {
        return -1;
    }

    size_t free_count = 0;

    size_t start = 0;

    for (size_t i = 0;
         i < map->size;
         i++)
    {
        if (!bitmap_test(
                map,
                i))
        {
             

            if (free_count == 0)
            {
                start = i;
            }

            free_count++;

             

            if (free_count >= count)
            {
                return (int)start;
            }
        }
        else
        {
             

            free_count = 0;
        }
    }

    return -1;
}