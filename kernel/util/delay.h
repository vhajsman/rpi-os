/*
    FILE:           kernel/delay.h
    ---------------------------------------------
    PARENT PROJECT: RpiOS
    CREDITS:        Vaclav Hajsman
    VERSION:        1.0
    REVISION:       0
    
    PURPOSE:        Delay
    
    ---------------------------------------------
    NOTES:          
*/

#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>

// Loop <delay> times in a way that the compiler won't optimize away
static inline void delay(int32_t count);

#endif
