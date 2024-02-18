#ifndef __UART_H
#define __UART_H

#include "kernel.h"

void uart_init(int board);

void uart_putc(unsigned char c);
void uart_puts(const char* str);

unsigned char uart_getc();

#endif