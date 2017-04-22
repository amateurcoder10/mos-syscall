# VersatilePB is a virtual ARM926EJ-S core with four UART ports
# 
# ARM926EJ-S processor implements the ARMv5TEJ instruction set which includes an
# enhanced 16 x 32-bit multiplier capable of single cycle MAC operations and
# 16-bit fixed point DSP instructions to enhance performance of many signal
# processing applications as well as supporting Thumb® technology.
# 
# The first port UART0 works as a terminal if the -nographic or -serial stdio
# options are used. This port is mapped to 0x101f0000 (versatilepb.c). The very
# first register at this address is the DR register. Writing to this address
# transmits data while incoming bytes can be read from it.
# 
# see http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183f/index.html
#     http://balau82.wordpress.com for blogs on bare metal programming

#
# To quit program, press CTRL-A and x.
#


CROSS_COMPILE=arm-none-eabi-
EMULATOR=

default : run

run: namaste.bin
	qemu-system-arm -M versatilepb -m 128M -nographic -kernel namaste.bin

startup.o : startup.s
	${CROSS_COMPILE}as -mcpu=arm926ej-s -g startup.s -o startup.o

namaste.o : namaste.c
	${CROSS_COMPILE}gcc -fno-builtin -mcpu=arm926ej-s -g -c namaste.c -o namaste.o

namaste.bin : namaste.o startup.o namaste.ld
	${CROSS_COMPILE}ld -T namaste.ld namaste.o startup.o -o namaste.elf
	${CROSS_COMPILE}objcopy -O binary namaste.elf namaste.bin
	-rm namaste.elf

runhello: hello.static
	qemu-arm hello.bin
hello.static: hello.c
	arm-linux-gnueabi-gcc -static -o hello.static hello.c

hello: hello.c
	arm-linux-gnueabi-gcc -o hello hello.c

hello64 : hello64.s
	gcc -nostdlib -o hello64 hello64.s

clean:
	-rm -f startup.o namaste.o namaste.bin namaste.elf hello.static hello hello.o hello64
.PHONY: run runhello clean
