# File:   Makefile
# Author: Sam Boyles, Jerry Yu, UC
# Date:   9 Oct 2017
# Descr:  Makefile for ENCE260 Assignment

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I../drivers -I../fonts -I../drivers/avr -I../utils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: assignment.out


# Compile: create object files from C source files.
assignment.o: assignment.c ../drivers/avr/ir_uart.h ../drivers/avr/system.h ../drivers/display.h ../drivers/navswitch.h ../fonts/font5x7_1.h ../utils/font.h ../utils/pacer.h ../utils/tinygl.h ../utils/game.h ../utils/transmission.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../drivers/avr/ir_uart.c ../drivers/avr/ir_uart.h ../drivers/avr/pio.h ../drivers/avr/system.h ../drivers/avr/timer0.h ../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../drivers/avr/pio.c ../drivers/avr/pio.h ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../drivers/avr/prescale.c ../drivers/avr/prescale.h ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../drivers/avr/system.c ../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../drivers/avr/timer.c ../drivers/avr/system.h ../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../drivers/avr/timer0.c ../drivers/avr/bits.h ../drivers/avr/prescale.h ../drivers/avr/system.h ../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../drivers/avr/usart1.c ../drivers/avr/system.h ../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../drivers/display.c ../drivers/avr/system.h ../drivers/display.h ../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../drivers/ledmat.c ../drivers/avr/pio.h ../drivers/avr/system.h ../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../drivers/navswitch.c ../drivers/avr/delay.h ../drivers/avr/pio.h ../drivers/avr/system.h ../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../utils/font.c ../drivers/avr/system.h ../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../utils/pacer.c ../drivers/avr/system.h ../drivers/avr/timer.h ../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../utils/tinygl.c ../drivers/avr/system.h ../drivers/display.h ../utils/font.h ../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

game.o: ../utils/game.c ../drivers/avr/system.h ../drivers/navswitch.h ../drivers/avr/ir_uart.h ../utils/tinygl.h ../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

transmission.o: ../utils/transmission.c ../drivers/avr/system.h ../drivers/avr/ir_uart.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
assignment.out: assignment.o ir_uart.o pio.o prescale.o system.o timer.o timer0.o usart1.o display.o ledmat.o navswitch.o font.o pacer.o tinygl.o game.o transmission.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: assignment.out
	$(OBJCOPY) -O ihex assignment.out assignment.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash assignment.hex; dfu-programmer atmega32u2 start


