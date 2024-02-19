#ifndef __MEMORY_H
#define __MEMORY_H

#include <stdint.h>
#include "atag.h"

typedef struct {
    uint32_t size;
    uint32_t start;
} mem_t;

typedef struct {
    uint32_t size;
    uint32_t start;
} initrd2_t;

typedef struct {
    char line[1];
} cmdline_t;

uint32_t getMemorySize(atag_t* tag);
void memory_init(atag_t* atags);

// --- PAGING ---
    #define PAGE_SIZE       4096

    extern uint32_t memory_pagesTotal;

    typedef struct {
        uint8_t allocated:          1;  // Allocated
        uint8_t kernel_heap_page:   1;
        uint8_t kernel_page:        1;  // Kernel reserved
        uint32_t reserved:          29; // Reserved
    } page_flags_t;

    typedef struct page {
        uint32_t vaddr_mapped;
        page_flags_t flags;

        DEFINE_LINK(page);
    } page_t;

    void* palloc(void);
    void pfree(void* ptr)

// --- MEMORY HEAP ---
    #define KERNEL_HEAP_SIZE    (1024*1024)
    #define KERNEL_STACK_SIZE   PAGE_SIZE
    
    typedef struct heap_segment{
        struct heap_segment* next;
        struct heap_segment* prev;

        uint32_t allocated;
        uint32_t segment_size;  // Includes this header
    } heap_segment_t;

void* kmalloc(uint32_t bytes);
void kfree(void *ptr)


#endif
