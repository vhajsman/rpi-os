#ifndef __KERNEL_H
#define __KERNEL_H

#define KVERSION "0.0.01.0a"

#include "kernel/board.h"
#include "kernel/memory/memory.h"
#include "kernel/utils/delay.h"

#include <libc/stddef.h>
#include <libc/stdint.h>

// SRC: kernel/mmio.c
    void mmio_detect();
    static inline void mmio_init(int board);
    static inline void mmio_write(uint32_t reg, uint32_t data);
    static inline uint32_t mmio_read(uint32_t reg);

    enum {
        GPIO_BASE,
        GPPUD,
        GPPUDCLK0,
        UART0_BASE,
        UART0_DR,
        UART0_RSRECR,
        UART0_FR,
        UART0_ILPR,
        UART0_IBRD,
        UART0_FBRD,
        UART0_LCRH,
        UART0_CR,
        UART0_IFLS,
        UART0_IMSC,
        UART0_RIS,
        UART0_MIS,
        UART0_ICR,
        UART0_DMACR,
        UART0_ITCR,
        UART0_ITIP,
        UART0_ITOP,
        UART0_TDR,
        MBOX_BASE,
        MBOX_READ,
        MBOX_STATUS,
        MBOX_WRITE
    };

// SRC: kernel/mailbox.c
    volatile unsigned int  mbox[9];

#endif