#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/dbgmcu.h>
#include <libopencm3/cm3/scs.h>
#include <libopencm3/cm3/tpiu.h>
#include <libopencm3/cm3/itm.h>

#include "init.h"

/*
 * To implement the STDIO functions you need to create
 * the _read and _write functions and hook them to the
 * USART you are using. This example also has a buffered
 * read function for basic line editing.
 */
int _write(int fd, char *ptr, int len);
int _read(int fd, char *ptr, int len);
void get_buffered_line(void);

#define BUFLEN 127

static uint16_t start_ndx;
static uint16_t end_ndx;
static char buf[BUFLEN + 1];
#define buf_len ((end_ndx - start_ndx) % BUFLEN)
static inline int inc_ndx(int n)
{
	return ((n + 1) % BUFLEN);
}
static inline int dec_ndx(int n) { return (((n + BUFLEN) - 1) % BUFLEN); }

static void clock_setup(void)
{
	rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_3V3_96MHZ]);
	rcc_periph_reset_pulse(RCC_AHB1RSTR);
	rcc_periph_reset_pulse(RCC_AHB2RSTR);
	rcc_periph_clock_enable(RCC_AHB1ENR);
	rcc_periph_clock_enable(RCC_AHB2ENR);

	/* Enable GPIOD    m
 clock for LED & USARTs. */
	rcc_periph_clock_enable(USART_GPIO_RCC);

	/* Enable clocks for USART. */
	rcc_periph_clock_enable(USART_RCC);
	return;
}

static void trace_setup(void)
{
	uint32_t SWOSpeed = 4500000;
	SCS_DEMCR |= SCS_DEMCR_TRCENA;
	DBGMCU_CR = DBGMCU_CR_TRACE_IOEN | DBGMCU_CR_TRACE_MODE_ASYNC;
	TPIU_SPPR = TPIU_SPPR_ASYNC_NRZ;
	TPIU_ACPR = (90000000 / SWOSpeed * 2) - 1;

	TPIU_FFCR &= ~TPIU_FFCR_ENFCONT;

	*((volatile uint32_t *)0xE0000FB0) = 0xC5ACCE55;
	ITM_TCR = (1 << 16) | ITM_TCR_ITMENA;
	/* Enable stimulus port 1. */
	ITM_TER[0] = 1;
}

static void usart_setup(void)
{
	/* Setup USART parameters. */
	usart_set_baudrate(USART, 115200);
	usart_set_databits(USART, 8);
	usart_set_stopbits(USART, USART_STOPBITS_1);
	usart_set_mode(USART, USART_MODE_TX);
	usart_set_parity(USART, USART_PARITY_NONE);
	usart_set_flow_control(USART, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART);
}

static void gpio_setup(void)
{
	/* Setup GPIO pins for USART transmit. */
	gpio_mode_setup(USART_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, USART_PINS);

	/* Setup USART TX pin as alternate function. */
	gpio_set_af(USART_PORT, USART_AF, USART_PINS);
}

static void trace_send_blocking(char c)
{
	while (!(ITM_STIM8(0) & ITM_STIM_FIFOREADY))
		;

	ITM_STIM8(0) = c;
}

void init(void)
{
	clock_setup();
	gpio_setup();
	usart_setup();
}

/* back up the cursor one space */
static inline void back_up(void)
{
	end_ndx = dec_ndx(end_ndx);
	usart_send_blocking(USART, '\010');
	usart_send_blocking(USART, ' ');
	usart_send_blocking(USART, '\010');
}

/*
 * A buffered line editing function.
 */
void get_buffered_line(void)
{
	char c;

	if (start_ndx != end_ndx)
	{
		return;
	}
	while (1)
	{
		c = usart_recv_blocking(USART);
		if (c == '\r')
		{
			buf[end_ndx] = '\n';
			end_ndx = inc_ndx(end_ndx);
			buf[end_ndx] = '\0';
			usart_send_blocking(USART, '\r');
			usart_send_blocking(USART, '\n');
			return;
		}
		/* ^H or DEL erase a character */
		if ((c == '\010') || (c == '\177'))
		{
			if (buf_len == 0)
			{
				usart_send_blocking(USART, '\a');
			}
			else
			{
				back_up();
			}
			/* ^W erases a word */
		}
		else if (c == 0x17)
		{
			while ((buf_len > 0) &&
				   (!(isspace((int)buf[end_ndx]))))
			{
				back_up();
			}
			/* ^U erases the line */
		}
		else if (c == 0x15)
		{
			while (buf_len > 0)
			{
				back_up();
			}
			/* Non-editing character so insert it */
		}
		else
		{
			if (buf_len == (BUFLEN - 1))
			{
				usart_send_blocking(USART, '\a');
			}
			else
			{
				buf[end_ndx] = c;
				end_ndx = inc_ndx(end_ndx);
				usart_send_blocking(USART, c);
			}
		}
	}
}

/*
 * Called by libc stdio fwrite functions
 */
int _write(int fd, char *ptr, int len)
{
	int i = 0;

	/*
	 * Write "len" of char from "ptr" to file id "fd"
	 * Return number of char written.
	 *
	 * Only work for STDOUT, STDIN, and STDERR
	 */
	if (fd > 2)
	{
		return -1;
	}
	while (*ptr && (i < len))
	{
		usart_send_blocking(USART, *ptr);
		if (*ptr == '\n')
		{
			usart_send_blocking(USART, '\r');
		}
		i++;
		ptr++;
	}
	return i;
}

/*
 * Called by the libc stdio fread fucntions
 *
 * Implements a buffered read with line editing.
 */
int _read(int fd, char *ptr, int len)
{
	int my_len;

	if (fd > 2)
	{
		return -1;
	}

	get_buffered_line();
	my_len = 0;
	while ((buf_len > 0) && (len > 0))
	{
		*ptr++ = buf[start_ndx];
		start_ndx = inc_ndx(start_ndx);
		my_len++;
		len--;
	}
	return my_len; /* return the length we got */
}