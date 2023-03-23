/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 12:13:48 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/23 15:08:03 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define LED_R PORTD5
#define LED_G PORTD6
#define LED_B PORTD3

void	init_rgb();
void	set_rgb(uint8_t r, uint8_t g, uint8_t b);
void	wheel(uint8_t pos);
void	init_timer0();
void	init_timer2();

int	main(void)
{
	init_rgb();
	for (;;)
	{
		for (uint8_t i = 0; i < 255; i++)
		{
			wheel(i);
			_delay_ms(5);
		}
	}
}

void	init_rgb()
{
	DDRD = 1 << DDD3 | 1 << DDD5 | 1 << DDD6;

	init_timer0();
	init_timer2();
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

void wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85) {
		set_rgb(255 - pos * 3, 0, pos * 3);
	} else if (pos < 170) {
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	} else {
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}
