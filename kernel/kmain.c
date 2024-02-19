#include <stdint.h>
#include "drivers/serial/uart.h"
#include "kernel/memory/memory.h"

#define UNUSED(X)   (void) X

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    UNUSED(r0);
    UNUSED(r1);
    
    // Initialize the memory.
    memory_init(atags);

    uart_init();

    while (1) {
        uart_putc(uart_getc());
        uart_putc('\n');
    }
}
