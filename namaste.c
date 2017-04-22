#include <stdint.h>
/*
 * UART0 is located at 0x101f1000 (versatilepb.c)
 * qemu emulates a subset of PL011 Prime cell UART.
 * The Tx/Rx register is located at offset 0.
 * By using -nographic or -serial stdio, this port
 * can be connected to stdio while running qemu-system-arm
 */

typedef struct {
	uint32_t dr;	// data ready
	uint32_t rsr_ecr;	// status register
	uint32_t  dummy1[4];
	const uint32_t fr;
	uint32_t dummy2[1];
	uint32_t lpr;
	uint32_t ibrd;
	uint32_t fbrd;
	uint32_t lcr_h;
	uint32_t cr;	// command register
	uint32_t ifls;	//
	uint32_t imsc;
	const uint32_t ris;
	const uint32_t mis;
	uint32_t icr;
	uint32_t dmacr;	// DMA command register
} pl011_dev;

enum {
	RXFE = 0x10, 	// receiver fifo empty
	TXFF = 0x22	// transmitter fifo full
};

volatile pl011_dev * const uart = (pl011_dev *)0x101f1000;
//volatile pl011_dev * const uart1 = (pl011_dev *)0x101f2000;
//volatile pl011_dev * const uart2 = (pl011_dev *)0x101f3000;

unsigned int mask = '*';

static void putc(char c)
{
	while (uart->fr & TXFF)
		;
	uart->dr = c;
}

static char getc()
{
	while (uart->fr & RXFE)
		;
	return (char)uart->dr;
}

static void puts(char *s)      { while (*s != '\0') putc(*s++); }

void c_entry()
{
	char buf[1024], c;
	int i;

	puts("Namaste!\n");
	puts("login : ");
	for (i = 0; i < 1024; i++) {
		buf[i] = getc();
		putc(buf[i]);
		if (buf[i] == '\r') break;
	}
	buf[i] = '\0';
	puts("\nSecret : ");
	while ((c = getc()) != '\r')
		putc(mask);
	puts("\nhello ");
	puts(buf);
	puts(", your secret is safe with me!\n\nPress ^A-x to exit");
}
