/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/21 16:35:50 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile int	value = 0;
volatile int	button1_value = 0;

ISR(INT0_vect)
{
	button1_value = !button1_value;
	_delay_ms(50);
	if (button1_value == 0)
		value = (value + 1) % 16;
	PORTB = ((value & 1) << PORTB0) | (((value & 2) >> 1) << PORTB1) | (((value & 4) >> 2) << PORTB2) | (((value & 8) >> 3) << PORTB4);
}

ISR(TIMER0_OVF_vect)
{
	int	old_button_value = PIND & (1 << PIND4);

	_delay_ms(50);
	if (old_button_value != (PIND & (1 << PIND4)))
		return ;
	value = (value + 15) % 16;
	PORTB = ((value & 1) << PORTB0) | (((value & 2) >> 1) << PORTB1) | (((value & 4) >> 2) << PORTB2) | (((value & 8) >> 3) << PORTB4);
}

int	main()
{
	// Interrupt from button 1
	EIMSK = 1 << INT0;		// Enable INT0 (Triggered by SW1)
	EICRA = 1 << ISC00;		// Interrupt when button pressed (when power goes down)

	// Interrupt from button 2 using timer 0
	TIMSK0 = 1 << TOIE0;			// Interrupt on overflow
	TCCR0A = 1 << WGM00 | 1 << WGM01;
	OCR0A = 0;						// Overflow at first tick
	TCCR0B = 1 << CS02 | 1 << CS01;	// Tick at button click
	TCCR0B |= 1 << WGM02;

	DDRB |= 1 << DDB0 | 1 << DDB1 | 1 << DDB2 | 1 << DDB4;
	sei();
	for (;;)
		;
}
