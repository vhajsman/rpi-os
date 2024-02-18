#ifndef __FRAMEBUFFER_H
#define __FRAMEBUFFER_H

#include <kernel.h>
#include "pixel.h"

#include <stdlib.h>
#include <stdint.h>

typedef enum {
    NULL_TAG =                      0x00000000,
    FB_ALLOCATE_BUFFER =            0x00040001,
    FB_RELESE_BUFFER =              0x00048001,
    FB_GET_PHYSICAL_DIMENSIONS =    0x00040003,
    FB_SET_PHYSICAL_DIMENSIONS =    0x00048003,
    FB_GET_VIRTUAL_DIMENSIONS =     0x00040004,
    FB_SET_VIRTUAL_DIMENSIONS =     0x00048004,
    FB_GET_BITS_PER_PIXEL =         0x00040005,
    FB_SET_BITS_PER_PIXEL =         0x00048005,
    FB_GET_BYTES_PER_ROW =          0x00040008
} property_tag_t;

typedef struct {
    void * fb_addr;
    uint32_t fb_size;
} fb_allocate_res_t;

typedef struct {
    uint32_t width;
    uint32_t height;
} fb_screen_size_t;

typedef union {
    uint32_t fb_allocate_align;
    fb_allocate_res_t fb_allocate_res;
    fb_screen_size_t fb_screen_size;
    uint32_t fb_bits_per_pixel;
    uint32_t fb_bytes_per_row;
} value_buffer_t;

typedef struct {
    property_tag_t proptag;
    value_buffer_t value_buffer;
} property_message_tag_t;


#endif
