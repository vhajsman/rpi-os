arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -c               boot.asm                -o boot.o
arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c    kernel/kmain.c          -o kmain.o -O2 -Wall -Wextra
arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c    kernel/util/delay.c     -o delay.o -O2 -Wall -Wextra
arm-none-eabi-gcc -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -c    drivers/serial/uart.c   -o uart.o -O2 -Wall -Wextra
arm-none-eabi-gcc -T linker.ld -o krnl.elf -ffreestanding -O2 -nostdlib *.o