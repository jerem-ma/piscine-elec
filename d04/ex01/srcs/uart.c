/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:25:38 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/22 22:15:05 by jmaia            ###   ###               */
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

void	uart_print_hex(uint8_t c)
{
	char * const letters = "0123456789ABCDEF";

	uart_printstr("0x");
	uart_tx(letters[c / 16]);
	uart_tx(letters[c % 16]);
}
