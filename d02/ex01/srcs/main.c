/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/21 14:28:01 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/interrupt.h>
#include <avr/io.h>

volatile int	switched = 0;

ISR(TIMER0_COMPA_vect)
{
	switched = 1;
}

int	main()
{
	int	duty_cycle = 0;
	int	direction = 1;

	DDRB |= 1 << PINB1;				// PB1 is now output mode. We use DDRB, because B0 is set in this register (cf. letter)

	// FREQUENCY TIMER
	TIMSK0 = 1 << OCIE0A;			// Interrupt on Compare Match
	TCCR0A = 1 << WGM01;			// Clear timer on compare (CTC)
	OCR0A = F_CPU / 1024 / 200;		// Compare match 200x times a second
	TCCR0B = 1 << CS02 | 1 << CS00;	// Set prescaler to 1024
	

	// LED TIMER
	TCCR1A = 1 << COM1A1;				// Clear on compare match, set at bottom

	TCCR1A |= 1 << WGM11;				// Use mode 14..
	TCCR1B = 1 << WGM13 | 1 << WGM12;	// ..
	
	TCCR1B |= 1 << CS12;	// Set prescaler to 256

	ICR1 = F_CPU / 256 / 200; 	// Set frequency to 1Hz
	OCR1A = ICR1 / 2;			// Define default duty cycle to 0%
	sei();						// Enable interrupt
	while (1)
	{
		if (switched)
		{
			int	new = OCR1A + direction * (ICR1 / 100);

			if (new > ICR1)
				new = ICR1;
			if (new == ICR1)
				direction = -1;
			if (direction == -1 && OCR1A < -direction * (ICR1 / 100))
				new = 0;
			if (new == 0)
				direction = 1;
			OCR1A = new;
			switched = 0;
		}
	}
}
