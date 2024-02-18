#ifndef __MEMORY_H
#define __MEMORY_H

#include "atag.h"
#include "heap.h"
#include "kernel.h"

#define MEMORY_PAGE_SIZE    4096

typedef struct {
    uint32_t size;
    uint32_t start;
} mem_t;

typedef struct {
    uint32_t start;
    uint32_t size;
} initrd2_t;

typedef struct {
    char line[1];
} cmdline_t;

typedef struct {
    uint8_t allocated: 1;
    uint8_t kernel_page: 1;
    uint32_t reserved: 30;
} page_flags_t;

typedef struct page {
    uint32_t vaddr_mapped;
    page_flags_t flags;

    DEFINE_LINK(page);
} page_t;

uint32_t memory_getSize(atag_t* atags);

void* allocate(void);
void free(void* ptr);
void* kmalloc(uint32_t bytes);
void kfree(void *ptr);

#endif