/*
    FILE:           kernel/mmio.h
    ---------------------------------------------
    PARENT PROJECT: RpiOS
    CREDITS:        Vaclav Hajsman
    VERSION:        1.1
    REVISION:       0
    
    PURPOSE:        Memory mapped input/output (MMIO)
    
    ---------------------------------------------
    NOTES:          
*/

#ifndef __MMIO_H
#define __MMIO_H

#include <stdint.h>

// Writes data to MMIO port
#define mmio_write(__reg, __data) \
    *(volatile uint32_t*) (uint32_t) __reg = (uint32_t) __data;

// Loads data from MMIO port
#define mmio_read(__reg) \
    (*(volatile uint32_t*) (uint32_t) __reg)

#endif