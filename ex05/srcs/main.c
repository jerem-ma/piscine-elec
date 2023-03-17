/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/17 18:16:44 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/common.h>
#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	int	value = 0;
	int	button1_value = 0;
	int	switched1 = 0;

	int	button2_value = 0;
	int	switched2 = 0;

	DDRB |= 1 << DDB0 | 1 << DDB1 | 1 << DDB2 | 1 << DDB4;
	for (;;)
	{
		switched1 = 0;
		if ((PIND & (1 << PIND2)) != button1_value)
		{
			switched1 = 1;
			button1_value = (PIND & (1 << PIND2));
			_delay_ms(50);
		}
		if (button1_value == 0 && switched1)
			value = (value + 1) % 16;

		switched2 = 0;
		if ((PIND & (1 << PIND4)) != button2_value)
		{
			switched2 = 1;
			button2_value = (PIND & (1 << PIND4));
			_delay_ms(50);
		}
		if (button2_value == 0 && switched2)
			value = (value - 1 + 16) % 16;

		
//		PORTB = (PORTB & ~(1 << PORTB0)) | (led_value << PORTB0); // Empty PB0, get value of PD2, set it to PB0
		PORTB = ((value & 1) << PORTB0) | (((value & 2) >> 1) << PORTB1) | (((value & 4) >> 2) << PORTB2) | (((value & 8) >> 3) << PORTB4);
	}
}
