/*
    FILE:           kernel/delay.c
    ---------------------------------------------
    PARENT PROJECT: RpiOS
    CREDITS:        Vaclav Hajsman
    VERSION:        1.0
    REVISION:       0
    
    PURPOSE:        Delay
    
    ---------------------------------------------
    NOTES:          
*/

#include "delay.h"

static inline void delay(int32_t count) {
    asm volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n" : "=r"(count): [count]"0"(count) : "cc");
}