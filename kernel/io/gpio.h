/*
    FILE:           kernel/io/gpio.h
    ---------------------------------------------
    PARENT PROJECT: RpiOS
    CREDITS:        Vaclav Hajsman
    VERSION:        1.0
    REVISION:       0
    
    PURPOSE:        GPIO Ports definition
    
    ---------------------------------------------
    NOTES:          
*/

#ifndef __GPIO_H
#define __GPIO_H

#if defined(___target_rpi2) || defined(___target_rpi3)
    #define __GPIO_BASE     0x3F200000
    #define __UART0_BASE    0x3F201000
#elif defined(___target_rpi1)
    #define __GPIO_BASE     0x20200000
    #define __UART0_BASE    0x20201000
#else
    #error "Target not supported."
#endif

#define GPIO_BASE       __GPIO_BASE
#define UART0_BASE      __UART0_BASE

#define GPPUD           (GPIO_BASE + 0x94)
#define GPPUDCLK0       (GPIO_BASE + 0x98)
#define UART0_DR        (UART0_BASE + 0x00)
#define UART0_RSRECR    (UART0_BASE + 0x04)
#define UART0_FR        (UART0_BASE + 0x18)
#define UART0_ILPR      (UART0_BASE + 0x20)
#define UART0_IBRD      (UART0_BASE + 0x24)
#define UART0_FBRD      (UART0_BASE + 0x28)
#define UART0_LCRH      (UART0_BASE + 0x2C)
#define UART0_CR        (UART0_BASE + 0x30)
#define UART0_IFLS      (UART0_BASE + 0x34)
#define UART0_IMSC      (UART0_BASE + 0x38)
#define UART0_RIS       (UART0_BASE + 0x3C)
#define UART0_MIS       (UART0_BASE + 0x40)
#define UART0_ICR       (UART0_BASE + 0x44)
#define UART0_DMACR     (UART0_BASE + 0x48)
#define UART0_ITCR      (UART0_BASE + 0x80)
#define UART0_ITIP      (UART0_BASE + 0x84)
#define UART0_ITOP      (UART0_BASE + 0x88)
#define UART0_TDR       (UART0_BASE + 0x8C)

#endif
