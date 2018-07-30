# Modify as appropriate
STELLARISWARE=C:/StellarisWare

CC=arm-none-eabi-gcc -Wall -Os -march=armv7-m -mcpu=cortex-m3 -mthumb -mfix-cortex-m3-ldrd -Wl,--gc-sections
	
lockdemo.elf: lockdemo.c syscalls.c startup_gcc.c threads.c create.S rit128x96x4.c
	${CC} -o $@ -I${STELLARISWARE} -L${STELLARISWARE}/driverlib/gcc -Tlinkscript.x -Wl,-Map,lockdemo.map -Wl,--entry,ResetISR lockdemo.c startup_gcc.c syscalls.c rit128x96x4.c create.S threads.c -ldriver

.PHONY: clean
clean:
	rm -f *.elf *.map

# vim: noexpandtab  
