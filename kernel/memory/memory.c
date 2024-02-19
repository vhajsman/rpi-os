#include "memory.h"

#include "list.h"
#include <stddef.h>
#include <stdlib.h>

extern uint8_t _end;
static uint32_t memory_pagesTotal;

static page_t*  allPages;
page_list_t     freePages;


uint32_t getMemorySize(atag_t* tag) {
    while(tag->tag != ATAG_NONE) {
        if(tag->tag == ATAG_MEM)
            return tag->mem.size;

        tag = ((uint32_t*) tag) + tag->size;
    }

    #ifdef ___VM_MODE
        // This can not be done in virtual machine.
        // This returns the size of 64 MB, which is
        // recommended size for running in virtual
        // machine.

        return 1024 * 1024 * 64;
    #else
        // Error
        return 0;
    #endif
}


DEFINE_LIST(page);
IMPLEMENT_LIST(page);


void memory_init(atag_t* atags) {
    uint32_t memorySize,  pageArray_len, kernelPages, i;

    memorySize = getMemorySize();
    allPages = memorySize / PAGE_SIZE;

    pageArray_len = sizeof(page_t * allPages);
    allPages = (page_t*) (&_end);

    bzero(allPages, pageArray_len);

    INITIALIZE_LIST(freePages);
    
    kernelPages = ((uint32_t) &_end) / PAGE_SIZE;
    for(i = 0; i < kernelPages; i++) {
        allPages[i].vaddr_mapped = i * PAGE_SIZE;

        allPages[i].flags.allocated = 1;
        allPages[i].flags.kernel_page = 1;
    }

    // Reserve 1 MB for the kernel heap
    for(; i < kernel_pages + (KERNEL_HEAP_SIZE / PAGE_SIZE); i++){
        allPages[i].vaddr_mapped = i * PAGE_SIZE;    // Identity map the kernel pages
        allPages[i].flags.allocated = 1;
        allPages[i].flags.kernel_heap_page = 1;
    }

    for(; i < memory_pagesTotal; i ++) {
        allPages[i].flags.allocated = 0;
        append_page_list(&freePages, &allPages[i]);
    }

    page_array_end = (uint32_t)&__end + page_array_len;
    heap_init(page_array_end)   
}

void* palloc(void) {
    page_t* page;
    void* page_mem;

    if(size_page_list(&freePages) == 0)
        return 0;

    page = pop_page_list(&freePages);
    
    page->flags.kernel_page = 1;
    page->flags.allocated = 1;

    page_mem = (void*) ((page - allPages) * PAGE_SIZE);

    bzero(page_mem, PAGE_SIZE);
    return page_mem;
}

void pfree(void* ptr) {
    page_t * page;
    page = allPages + ((uint32_t) ptr / PAGE_SIZE);

    page->flags.allocated = 0;
    append_page_list(&freePages, page);
}

static heap_segment_t * heap_segment_list_head;

void* kmalloc(uint32_t bytes) {
    heap_segment_t *curr, *best = NULL;
    int diff, best_diff = INT32_MAX;

    bytes += sizeof(heap_segment_t);
    bytes += bytes % 16 ? 16 - (bytes % 16) : 0;

    for(curr = heap_segment_list_head; curr != NULL; curr = curr->next) {
        diff = curr->segment_size - bytes;

        if (!curr->allocated && diff < best_diff && diff >= 0) {
            best = curr;
            best_diff = diff;
        }
    }

    // Not enough memory
    if (best == NULL) {
        // TODO: handle the error

        return NULL;
    }

    // If the best difference we could come up with was large, split up this segment into two.
    if (best_diff > (int) (2 * sizeof(heap_segment_t))) {
        bzero(((void*) (best)) + bytes, sizeof(heap_segment_t));
        curr = best->next;

        best->next = ((void*)(best)) + bytes;
        best->next->next = curr;
        best->next->prev = best;
        best->next->segment_size = best->segment_size - bytes;

        best->segment_size = bytes;
    }

    best->allocated = 1;
    return best + 1;
}

void kfree(void *ptr) {
    heap_segment_t * seg = ptr - sizeof(heap_segment_t);
    seg->is_allocated = 0;

    while(seg->prev != NULL && !seg->prev->allocated) {
        seg->prev->next = seg->next;
        seg->prev->segment_size += seg->segment_size;

        seg = seg->prev;
    }
    
    while(seg->next != NULL && !seg->next->allocated) {
        seg->segment_size += seg->next->segment_size;

        seg->next = seg->next->next;
    }
}

static void heap_init(uint32_t heap_start) {
    heap_segment_list_head = (heap_segment_t *) heap_start;
    bzero(heap_segment_list_head, sizeof(heap_segment_t));
    
    heap_segment_list_head->segment_size = KERNEL_HEAP_SIZE;
}


