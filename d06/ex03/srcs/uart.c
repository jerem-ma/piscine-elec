/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:25:38 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/23 15:38:49 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include "uart.h"

void	uart_init(void)
{
	UBRR0L = my_round(F_CPU / (16 * UART_BAUDRATE)); // Set baudrate
	UCSR0B = (1 << TXEN0 | 1 << RXEN0); // Enable transmission of data
	// Default is 8N1 frame format (cf. p201)
}

int	my_round(double nbr)
{
	if (nbr - (int) nbr < 0.5)
		return (int) nbr;
	return (int) nbr + 1;
}

void	uart_printstr(char const *str)
{
	while (*str)
	{
		uart_tx(*str);
		str++;
	}
}

void	uart_tx(unsigned char c)
{
	// Wait until transmit buffer empty (cf. p186 and p200)
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = c;
}

char	uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0))) // Wait for a character to be sent
		;

	return UDR0;
}

uint8_t	uart_getline(uint8_t *line, uint8_t size)
{
	uint8_t	c;
	uint8_t	i;

	c = 0;
	for (i = 0; c != '\r'; i++)
	{
		c = uart_rx();
		if (c == '\x7F') // DEL
		{
			if (i != 0)
			{
				uart_delchar();
				i -= 2;
			}
		}
		else if (i < size)
			line[i] = c;
		if (c != '\x7F')
			uart_tx(c);
	}
	uart_printstr("\n\r");
	line[i >= size ? size - 1 : i] = 0;
	return i - 1;
}

void	uart_delchar()
{
	uart_printstr("\b \b");
}

void	uart_print_hex(unsigned char c)
{
	char * const letters = "0123456789ABCDEF";

	uart_printstr("0x");
	uart_tx(letters[c / 16]);
	uart_tx(letters[c % 16]);
}

// This function shall only be called with 2 digits numbers
void	uart_putnbr2(int32_t nbr)
{
	if (nbr < 0)
	{
		uart_tx('-');
		nbr = -nbr;
	}
	if (nbr >= 100)
	{
		uart_printstr("NUMBER_TOO_LARGE");
		return ;
	}
	uart_tx(nbr / 10 + '0');
	uart_tx(nbr % 10 + '0');
}

void	uart_putnbr2_double2(double nbr)
{
	if (nbr >= 100)
	{
		uart_printstr("NUMBER_TOO_LARGE");
		return ;
	}
	uart_putnbr2(nbr);
	uart_tx('.');
	uart_putnbr2(((int32_t)(nbr * 100)) % 100);
}
