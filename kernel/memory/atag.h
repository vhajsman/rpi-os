#ifndef __ATAG_H
#define __ATAG_H

#include "memory.h"

typedef enum {
    ATAG_NONE       = 0x00000000,
    ATAG_CORE       = 0x54410001,
    ATAG_MEM        = 0x54410002,
    ATAG_INITRD2    = 0x54420005,
    ATAG_CMDLINE    = 0x54410009,
} atag_tag_t;

typedef struct atag {
    uint32_t    size;
    atag_tag_t  tag;

    union {
        mem_t       mem;
        initrd2_t   initrd2;
        cmdline_t   cmdline;
    }
} atag_t;

#endif
