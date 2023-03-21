/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/21 18:52:40 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

void	uart_init(void);
int		my_round(double nbr);
char	uart_rx(void);
void	uart_tx(unsigned char c);

int	main()
{
	char	c;

	uart_init();
	
	while (1)
	{
		c = uart_rx();
		uart_tx(c);
	}
}

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
