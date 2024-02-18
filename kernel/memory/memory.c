#include "memory.h"

#include <stdlib.h>
#include "kernel/events.h"

#define ERR_NOT_ENOUGH_MEMORY   601

extern uint8_t __end;
static uint32_t num_pages;

static heap_segment_t * heap_segment_list_head;

DEFINE_LIST(page);
IMPLEMENT_LIST(page);

static page_t*  allPages;
page_list_t     freePages;


uint32_t memory_getSize(atag_t* tag) {
    while(tag->tag != NONE) {
        if(tag->tag == MEM)
            return tag->mem.size;

        tag = ((uint32_t*) tag) + tag->tag_size;
    }

    return 0;
}

void memory_init(atag_t* atags) {
    uint32_t mem_size,  pageArray_len, kernel_pages, i;

    mem_size = get_mem_size(atags);
    num_pages = mem_size / MEMORY_PAGE_SIZE;

    pageArray_len = sizeof(page_t) *num_pages;
    allPages = (page_t*) &__end;

    bzero(allPages, pageArray_len);
    INITIALIZE_LIST(freePages);

    kernel_pages = ((uint32_t)&__end) / MEMORY_PAGE_SIZE;
    for (i = 0; i < kernel_pages; i++) {
        allPages[i].vaddr_mapped = i * MEMORY_PAGE_SIZE;
        allPages[i].flags.allocated = 1;
        allPages[i].flags.kernel_page = 1;
    }
    
    for(; i < num_pages; i++){
        allPages[i].flags.allocated = 0;
        append_page_list(&free_pages, &allPages[i]);
    }

    // Reserve 1 MB for the kernel heap
    for(; i < kernel_pages + (KERNEL_HEAP_SIZE / MEMORY_PAGE_SIZE); i++) {
        all_pages_array[i].vaddr_mapped = i * MEMORY_PAGE_SIZE;
        all_pages_array[i].flags.allocated = 1;
        all_pages_array[i].flags.kernel_heap_page = 1;
    }

    // Initialize the heap
    page_array_end = (uint32_t)&__end + page_array_len;
    heap_init(page_array_end);
}

void* allocate(void) {
    page_t *page;
    void *page_mem;

    if (size_page_list(&freePages) == 0)
        return 0;


    page = pop_page_list(&freePages);

    page->flags.kernel_page = 1;
    page->flags.allocated = 1;


    page_mem = (void *)((page - allPages) * MEMORY_PAGE_SIZE);
    bzero(page_mem, MEMORY_PAGE_SIZE);

    return page_mem;
}


void free_page(void* ptr) {
    page_t * page;

    page = all_pages_array + ((uint32_t)ptr / MEMORY_PAGE_SIZE);

    page->flags.allocated = 0;
    append_page_list(&free_pages, page);
}

void* kmalloc(uint32_t bytes) {
    heap_segment_t * curr, *best = NULL;
    int diff, best_diff = 0x7fffffff;

    bytes += sizeof(heap_segment_t);
    bytes += bytes % 16 ? 16 - (bytes % 16) : 0;

    for (curr = heap_segment_list_head; curr != NULL; curr = curr->next) {
        diff = curr->segment_size - bytes;

        if (!curr->is_allocated && diff < best_diff && diff >= 0) {
            best = curr;
            best_diff = diff;
        }
    }

    // Not enough memory.
    if (best == NULL) {
        event_t _ev = Event("kmalloc()", "not enough memory", ERR_NOT_ENOUGH_MEMORY);
        REPORT_EVENT(_ev, false);

        return NULL;
    }

    if (best_diff > (int) (2 * sizeof(heap_segment_t))) {
        bzero(((void*) (best)) + bytes, sizeof(heap_segment_t));

        curr = best->next;

        best->next = ((void*)(best)) + bytes;
        best->next->next = curr;
        best->next->prev = best;
        best->next->segment_size = best->segment_size - bytes;

        best->segment_size = bytes;
    }

    best->is_allocated = 1;
    return best + 1;
}

void kfree(void *ptr) {
    heap_segment_t * seg = ptr - sizeof(heap_segment_t);
    seg->is_allocated = 0;

    while(seg->prev != NULL && !seg->prev->is_allocated) {
        seg->prev->next = seg->next;
        seg->prev->segment_size += seg->segment_size;

        seg = seg->prev;
    }
    
    while(seg->next != NULL && !seg->next->is_allocated) {
        seg->segment_size += seg->next->segment_size;
        seg->next = seg->next->next;
    }
}

