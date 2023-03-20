/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/20 19:13:37 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int	switched = 0;

ISR(INT0_vect)
{
	if (!switched)
		switched = 1;
}

int	main()
{
	int	led_value = 1;
	int	button_value = 1;

	DDRB |= 1 << DDB0;	// PB0 is now output mode. We use DDRB, because B0 is set in this register (cf. letter)

	EIMSK = 1 << INT0;	// Enable INT0
	EICRA = 1 << ISC00;	// Interrupt when button pressed (when power goes down)
	sei();

	for (;;)
	{
		if (switched)
		{
			button_value = !button_value;
			_delay_ms(50);
			if (button_value == 0)
				led_value = !led_value;
			switched = 0;
		}
		PORTB = (PORTB & ~(1 << PORTB0)) | (led_value << PORTB0); // Empty PB0, get value of PD2, set it to PB0
	}
}
