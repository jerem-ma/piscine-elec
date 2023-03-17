/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/17 18:06:26 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/common.h>
#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	int	led_value = 1;
	int	button_value = 0;
	int	switched = 0;

	DDRB |= 1 << DDB0;			// PB0 is now output mode. We use DDRB, because B0 is set in this register (cf. letter)
	for (;;)
	{
		switched = 0;
		if ((PIND & (1 << PIND2)) != button_value)
		{
			switched = 1;
			button_value = (PIND & (1 << PIND2));
			_delay_ms(50);
		}
		if (button_value == 0 && switched)
			led_value = !led_value;
		PORTB = (PORTB & ~(1 << PORTB0)) | (led_value << PORTB0); // Empty PB0, get value of PD2, set it to PB0
	}
}
