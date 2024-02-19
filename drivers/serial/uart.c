/*
    FILE:           drivers/serial/uart.c
    ---------------------------------------------
    PARENT PROJECT: RpiOS
    CREDITS:        Vaclav Hajsman
    VERSION:        1.0
    REVISION:       0
    
    PURPOSE:        UART communication driver
    
    ---------------------------------------------
    NOTES:          
*/

#include "kernel/util/delay.h"
#include "kernel/io/mmio.h"
#include "kernel/io/gpio.h"

uint32_t uart_baudrate;

#define uart_read() \
    mmio_read(UART0_DR)
#define uart_writeByte(byte) \
    mmio_write(UART0_DR, byte)

void uart_init() {
    uart_baudrate = 115200;

    uart_disable();

    mmio_write(GPPUD, 0x00000000);
    delay(150);

    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);

    mmio_write(GPPUDCLK0, 0x00000000);

    uart_clearInterrupts();

    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);

    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));

    uart_disableInterrupts();
    uart_enable_txrx();
}

void uart_putc(unsigned char c) {
    uart_waitOutBuff();
    uart_writeByte(c);
}

unsigned char uart_getc() {
    uart_waitInBuff();
    return uart_read();
}

void uart_puts(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i ++)
        uart_putc((unsigned char) str[i]);
}