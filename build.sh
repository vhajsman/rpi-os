aarch64-elf-gcc -ffreestanding -c $(find -name "*.c") -O2 -Wall -Wextra
aarch64-elf-as -c $(find -name "*.s")

mv *.o ./build
mv *.out ./build

aarch64-elf-gcc -T kernel.ld -o build/kernel.elf -ffreestanding -O2 -nostdlib build/*.o build/*.out -lgcc 
aarch64-elf-objcopy build/kernel.elf -O binary build/kernel8.img