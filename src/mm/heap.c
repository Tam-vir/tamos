#include "stdio.h"
#include "errno.h"
#include "heap.h"
#include "memory.h"
#include "spinlock.h"

#define HEAP_MIN_BLOCK_SIZE 32



static heap_block_t *heap_head = 0;

static size_t used_bytes = 0;

static spinlock_t heap_lock;



static size_t align_size(
    size_t size)
{
    return (size + 7) &
           ~(size_t)7;
}



static void heap_split(
    heap_block_t *block,
    size_t size)
{
    if (block->size <=
        size +
            sizeof(heap_block_t) +
            HEAP_MIN_BLOCK_SIZE)
    {
        return;
    }

    heap_block_t *new_block =
        (heap_block_t *)((uint8_t *)block +
                         sizeof(heap_block_t) +
                         size);

    new_block->size =
        block->size -
        size -
        sizeof(heap_block_t);

    new_block->free = 1;

    new_block->next =
        block->next;

    block->size =
        size;

    block->next =
        new_block;
}



static void heap_merge(void)
{
    heap_block_t *current =
        heap_head;

    while (current &&
           current->next)
    {
        if (current->free &&
            current->next->free)
        {
            current->size +=
                sizeof(heap_block_t) +
                current->next->size;

            current->next =
                current->next->next;
        }
        else
        {
            current =
                current->next;
        }
    }
}



void heap_init(void)
{
    spinlock_init(
        &heap_lock);

    heap_head =
        (heap_block_t *)
            heap_start();

    heap_head->size =
        heap_total() -
        sizeof(heap_block_t);

    heap_head->free = 1;

    heap_head->next = 0;

    used_bytes = 0;

    errno = SUCCESS;
}



void *heap_alloc(
    size_t size)
{
    if (size == 0)
    {
        errno = EINVAL;
        return 0;
    }

    size =
        align_size(size);

    spinlock_lock(
        &heap_lock);

    heap_block_t *current =
        heap_head;

    while (current)
    {
        if (current->free &&
            current->size >= size)
        {

            heap_split(
                current,
                size);

            current->free = 0;

            used_bytes +=
                current->size;

            void *ptr =
                (void *)((uint8_t *)current +
                         sizeof(heap_block_t));

            spinlock_unlock(
                &heap_lock);

            errno = SUCCESS;

            return ptr;
        }

        current =
            current->next;
    }

    spinlock_unlock(
        &heap_lock);

    errno = ENOMEM;

    return 0;
}



void heap_free(
    void *ptr)
{
    if (!ptr)
    {
        errno = EINVAL;
        return;
    }

    spinlock_lock(
        &heap_lock);

    heap_block_t *block =
        (heap_block_t *)((uint8_t *)ptr -
                         sizeof(heap_block_t));

    if (!block->free)
    {
        used_bytes -=
            block->size;

        block->free = 1;

        heap_merge();

        errno = SUCCESS;
    }
    else
    {
        errno = EINVAL;
    }

    spinlock_unlock(
        &heap_lock);
}



size_t heap_block_size(
    void *ptr)
{
    if (!ptr)
    {
        errno = EINVAL;
        return 0;
    }

    spinlock_lock(
        &heap_lock);

    heap_block_t *block =
        (heap_block_t *)((uint8_t *)ptr -
                         sizeof(heap_block_t));

    size_t size =
        block->size;

    spinlock_unlock(
        &heap_lock);

    errno = SUCCESS;

    return size;
}



size_t heap_total(void)
{
    return (size_t)((uint8_t *)heap_end() -
                    (uint8_t *)heap_start());
}



size_t heap_used(void)
{
    spinlock_lock(
        &heap_lock);

    size_t value =
        used_bytes;

    spinlock_unlock(
        &heap_lock);

    return value;
}



size_t heap_available_size(void)
{
    spinlock_lock(
        &heap_lock);

    size_t total = 0;

    heap_block_t *current =
        heap_head;

    while (current)
    {
        if (current->free)
        {
            total +=
                current->size;
        }

        current =
            current->next;
    }

    spinlock_unlock(
        &heap_lock);

    return total;
}



size_t heap_largest_free_block(void)
{
    spinlock_lock(
        &heap_lock);

    size_t largest = 0;

    heap_block_t *current =
        heap_head;

    while (current)
    {
        if (current->free &&
            current->size > largest)
        {
            largest =
                current->size;
        }

        current =
            current->next;
    }

    spinlock_unlock(
        &heap_lock);

    return largest;
}



size_t heap_free_blocks(void)
{
    spinlock_lock(
        &heap_lock);

    size_t count = 0;

    heap_block_t *current =
        heap_head;

    while (current)
    {
        if (current->free)
        {
            count++;
        }

        current =
            current->next;
    }

    spinlock_unlock(
        &heap_lock);

    return count;
}



void heap_dump(void)
{
    spinlock_lock(
        &heap_lock);

    heap_block_t *current =
        heap_head;

    size_t count = 0;

    size_t available = 0;

    size_t largest = 0;

    size_t free_blocks = 0;

    kprintf(
        "\n========== HEAP ==========\n");

    while (current)
    {

        kprintf(
            "Block %u\n",
            (uint64_t)count);

        kprintf(
            "Address : 0x%x\n",
            (uint64_t)current);

        kprintf(
            "Size    : %u bytes\n",
            (uint64_t)current->size);

        kprintf(
            "Free    : %u\n\n",
            (uint64_t)current->free);

        if (current->free)
        {
            available +=
                current->size;

            free_blocks++;

            if (current->size >
                largest)
            {
                largest =
                    current->size;
            }
        }

        count++;

        current =
            current->next;
    }

    kprintf(
        "--------------------------\n");

    kprintf(
        "Heap Total         : %u bytes\n",
        (uint64_t)heap_total());

    kprintf(
        "Heap Used          : %u bytes\n",
        (uint64_t)used_bytes);

    kprintf(
        "Heap Available     : %u bytes\n",
        (uint64_t)available);

    kprintf(
        "Largest Free Block : %u bytes\n",
        (uint64_t)largest);

    kprintf(
        "Free Blocks        : %u\n",
        (uint64_t)free_blocks);

    kprintf(
        "==========================\n\n");

    spinlock_unlock(
        &heap_lock);
}