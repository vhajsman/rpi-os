/*
    FILE:           drivers/serial/uart.h
    ---------------------------------------------
    PARENT PROJECT: RpiOS
    CREDITS:        Vaclav Hajsman
    VERSION:        1.0
    REVISION:       0
    
    PURPOSE:        UART communication driver
    
    ---------------------------------------------
    NOTES:          
*/

#ifndef __UART_H
#define __UART_H

extern uint32_t uart_baudrate;

// Disables all aspects of UART hardware
#define uart_disable() \
    mmio_write(UART0_CR, 0x00000000)

// Clears all pending interrupts from the UART hardware
#define uart_clearInterrupts() \
    mmio_write(UART0_ICR, 0x7FF)

/*void uart_setBaudrate(uint32_t baudrate) {
    uint32_t _divisor = UART_CLOCK_SPEED/(16 * baudrate);
}*/

#define uart_disableInterrupts()                            \
    mmio_write(UART0_IMSC,                                  \
            (1 << 1) | (1 << 4) | (1 << 5) | (1 << 6) |     \
            (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10))

#define uart_enable_txrx() \
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9))

// Wait for output buffer to be empty
#define uart_waitOutBuff() \
    while(mmio_read(UART0_FR) & (1 << 5)) { }

//Wait for input buffer to be empty
#define uart_waitInBuff() \
    while ( mmio_read(UART0_FR) & (1 << 4) ) { }

void uart_init();
void uart_putc(unsigned char c);
unsigned char uart_getc();
void uart_puts(const char* str);


#endif
