#include "kernel.h"
#include <stdint.h>

char* board_name;

uint32_t reg;
uint32_t* mmio_base;

void mmio_detect() {
    #if __AARCH64__
        asm volatile ("mrs %x0, midr_el1" : "=r" (reg));
    #else
        asm volatile ("mrc p15,0,%0,c0,c0,0" : "=r" (reg));
    #endif

    switch ((reg >> 4) & 0xFFF) {
        case 0xB76: 
            board = "Rpi1"; 
            mmio_base = 0x20000000; 
            
            break;

        case 0xC07: 
            board = "Rpi2"; 
            mmio_base = 0x3F000000; 
            
            break;

        case 0xD03: 
            board = "Rpi3"; 
            mmio_base = 0x3F000000; 
            
            break;

        case 0xD08: 
            board = "Rpi4"; 
            mmio_base = 0xFE000000; 
            
            break;

        default:
            board = "unknown board"; 
            mmio_base = 0x20000000; 
            
            break;
    }
}

static inline void mmio_init(int board) {
    mmio_detect();
}

static inline void mmio_write(uint32_t reg, uint32_t data) {
    *(volatile uint32_t*)(MMIO_BASE + reg) = data;
}

static inline uint32_t mmio_read(uint32_t reg) {
    return *(volatile uint32_t*)(MMIO_BASE + reg);
}

// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count) {
    asm volatile(
        "__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
        : "=r"(count) 
        : [count] "0"(count) 
        : "cc"
    );
}

enum {
    GPIO_BASE =     0x200000,
    GPPUD =         (GPIO_BASE + 0x94),
    GPPUDCLK0 =     (GPIO_BASE + 0x98),
    UART0_BASE =    (GPIO_BASE + 0x1000),
    UART0_DR =      (UART0_BASE + 0x00),
    UART0_RSRECR =  (UART0_BASE + 0x04),
    UART0_FR =      (UART0_BASE + 0x18),
    UART0_ILPR =    (UART0_BASE + 0x20),
    UART0_IBRD =    (UART0_BASE + 0x24),
    UART0_FBRD =    (UART0_BASE + 0x28),
    UART0_LCRH =    (UART0_BASE + 0x2C),
    UART0_CR =      (UART0_BASE + 0x30),
    UART0_IFLS =    (UART0_BASE + 0x34),
    UART0_IMSC =    (UART0_BASE + 0x38),
    UART0_RIS =     (UART0_BASE + 0x3C),
    UART0_MIS =     (UART0_BASE + 0x40),
    UART0_ICR =     (UART0_BASE + 0x44),
    UART0_DMACR =   (UART0_BASE + 0x48),
    UART0_ITCR =    (UART0_BASE + 0x80),
    UART0_ITIP =    (UART0_BASE + 0x84),
    UART0_ITOP =    (UART0_BASE + 0x88),
    UART0_TDR =     (UART0_BASE + 0x8C),
    MBOX_BASE =     0xB880,
    MBOX_READ =     (MBOX_BASE + 0x00),
    MBOX_STATUS =   (MBOX_BASE + 0x18),
    MBOX_WRITE =    (MBOX_BASE + 0x20)
};
