#ifndef __PERIPHERIAL_H
#define __PERIPHERIAL_H

#if defined(___target_rpi1)
    #define PERIPHERAL_BASE 0x20000000
#else
    #define PERIPHERAL_BASE 0x3F000000
#endif

#define PERIPHERAL_LENGTH   0x01000000
#define SYSTEM_TIMER_OFFSET 0x3000
#define INTERRUPTS_OFFSET   0xB000
#define MAILBOX_OFFSET      0xB880
#define UART0_OFFSET        0x201000
#define GPIO_OFFSET         0x200000
#define EMMC_OFFSET         0x300000

#endif