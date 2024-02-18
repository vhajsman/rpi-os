#include "heap.h"

static void heap_init(uint32_t heap_start) {
    heap_segment_list_head = (heap_segment_t *) heap_start;
    bzero(heap_segment_list_head, sizeof(heap_segment_t));
    heap_segment_list_head->segment_size = KERNEL_HEAP_SIZE;
}