/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 12:13:48 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/23 14:46:17 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define LED_R PORTD5
#define LED_G PORTD6
#define LED_B PORTD3

int	main(void)
{
	DDRD = 1 << DDD3 | 1 << DDD5 | 1 << DDD6;

	for (;;)
	{
		PORTD = 1 << LED_R;
		_delay_ms(1000);
		PORTD = 1 << LED_G;
		_delay_ms(1000);
		PORTD = 1 << LED_B;
		_delay_ms(1000);
		PORTD = 1 << LED_R | 1 << LED_G;
		_delay_ms(1000);
		PORTD = 1 << LED_G | 1 << LED_B;
		_delay_ms(1000);
		PORTD = 1 << LED_B | 1 << LED_R;
		_delay_ms(1000);
		PORTD = 1 << LED_B | 1 << LED_R | 1 << LED_G;
		_delay_ms(1000);
	}
}
