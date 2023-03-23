/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 12:13:48 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/23 15:40:52 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"

#define LED_R PORTD5
#define LED_G PORTD6
#define LED_B PORTD3

void	init_rgb();
int		get_rgb(uint8_t *r, uint8_t *g, uint8_t *b);
uint8_t	get_hex_value(char c);
void	set_rgb(uint8_t r, uint8_t g, uint8_t b);
void	init_timer0();
void	init_timer2();

int	main(void)
{
	uint8_t r, g, b;
	uint8_t	err;

	uart_init();
	init_rgb();
	for (;;)
	{
		err = get_rgb(&r, &g, &b);
		if (!err)
			set_rgb(r, g, b);
	}
}

void	init_rgb()
{
	DDRD = 1 << DDD3 | 1 << DDD5 | 1 << DDD6;

	init_timer0();
	init_timer2();
}

int	get_rgb(uint8_t *r, uint8_t *g, uint8_t *b)
{
	uint8_t	line[8];
	uint8_t	size;

	size = uart_getline(line, 8);
	if (line[0] != '#')
	{
		uart_printstr("Syntax error. Syntax: #RRGGBB\n\r");
		return (1);
	}
	if (size != 7)
	{
		uart_printstr("Syntax error. Syntax: #RRGGBB\n\r");
		return (1);
	}
	for (int i = 1; i < 7; i++)
	{
		if ((line[i] < '0' || line[i] > '9') && (line[i] < 'A' || line[i] > 'F'))
		{
			uart_printstr("Syntax error. Syntax: #RRGGBB\n\r");
			return (1);
		}
	}
	*r = get_hex_value(line[1]) * 16 + get_hex_value(line[2]);
	*g = get_hex_value(line[3]) * 16 + get_hex_value(line[4]);
	*b = get_hex_value(line[5]) * 16 + get_hex_value(line[6]);
	return 0;
}

uint8_t	get_hex_value(char c)
{
	char	*letters = "0123456789ABCDEFG";

	for (uint8_t i = 0; i < 16; i++)
	{
		if (c == letters[i])
			return i;
	}
	return -1;
}

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0A = g;
	OCR0B = r;
	OCR2B = b;
}

void	init_timer0()
{
	TCCR0A = 1 << WGM01 | 1 << WGM00 | 1 << COM0A1 | 1 << COM0B1;
	TCCR0B = 1 << CS02;
}

void	init_timer2()
{
	TCCR2A = 1 << WGM21 | 1 << WGM20 | 1 << COM2B1;
	TCCR2B = 1 << CS22;
}
