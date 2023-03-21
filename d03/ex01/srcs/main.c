/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/21 18:37:22 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void	uart_init(void);
int		my_round(double nbr);
void	uart_printstr(char const *str);
void	uart_tx(unsigned char c);

ISR(TIMER1_COMPA_vect)
{
	uart_printstr("Hello World!\r\n");
}

int	main()
{
	uart_init();
	
	// Init timer
	TCCR1B = 1 << WGM12;				// Set mode to CTC
	TCCR1B |= 1 << CS12 | 1 << CS10;	// Set prescaler to 1024
	OCR1A = F_CPU / 1024 / 0.5;			// Compare match every 2s
	TIMSK1 = 1 << OCIE1A;				// Enable timer1 interrupt
	sei();								// Enable interrupt

	while (1)
		;
}

void	uart_init(void)
{
	UBRR0L = my_round(F_CPU / (16 * UART_BAUDRATE)); // Set baudrate
	UCSR0B = (1 << TXEN0); // Enable transmission of data
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
