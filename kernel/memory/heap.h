#ifndef __HEAP_H
#define __HEAP_H

typedef struct heap_segment{
    struct heap_segment * next;
    struct heap_segment * prev;
    uint32_t is_allocated;
    uint32_t segment_size;
} heap_segment_t;

static void heap_init(uint32_t heap_start);

#endif