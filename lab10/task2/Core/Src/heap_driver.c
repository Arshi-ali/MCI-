#include "heap_driver.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#define HEAP_START_ADDR  ((uint8_t*)0x20001000)
#define HEAP_SIZE        (4 * 1024)
#define BLOCK_SIZE       16
#define BLOCK_COUNT      (HEAP_SIZE / BLOCK_SIZE)

// Students should be provided the above code (includes and defines) and the function declarations in this file.
// They can figure out the rest.

// Allocation bitmap: 0 = free, 1 = used

// Add you code below


static uint8_t block_map[BLOCK_COUNT];
static uint16_t block_length[BLOCK_COUNT];

void heap_init(void)
{
    for (int i = 0; i < BLOCK_COUNT; i++)
    {
        block_map[i] = 0;
        block_length[i] = 0;
    }
}

void* heap_alloc(size_t size)
{
    if (size == 0 || size > HEAP_SIZE)
        return NULL;

    size_t needed = (size + BLOCK_SIZE - 1) / BLOCK_SIZE;

    for (size_t i = 0; i <= BLOCK_COUNT - needed; i++)
    {
        size_t j;

        for (j = 0; j < needed; j++)
        {
            if (block_map[i + j] != 0)
                break;
        }

        if (j == needed)
        {
            for (j = 0; j < needed; j++)
                block_map[i + j] = 1;

            block_length[i] = needed;
            return (void*)(HEAP_START_ADDR + i * BLOCK_SIZE);
        }
    }
    return NULL;
}

void heap_free(void* ptr)
{
    if (ptr == NULL)
        return;

    uintptr_t addr = (uintptr_t)ptr;
    uintptr_t start = (uintptr_t)HEAP_START_ADDR;

    if (addr < start || addr >= start + HEAP_SIZE)
        return;

    size_t offset = addr - start;

    if (offset % BLOCK_SIZE != 0)
        return;

    size_t index = offset / BLOCK_SIZE;
    size_t count = block_length[index];

    if (count == 0)
        return;

    for (size_t i = 0; i < count; i++)
        block_map[index + i] = 0;

    block_length[index] = 0;
}
