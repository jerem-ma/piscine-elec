/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/20 17:05:15 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>

int	main()
{
	DDRB |= 1 << PINB1;								// PB1 is now output mode. We use DDRB, because B0 is set in this register (cf. letter)

	TCCR1A = 1 << COM1A1;				// Clear on compare match, set at bottom

	TCCR1A |= 1 << WGM11;				// Use mode 14..
	TCCR1B = 1 << WGM13 | 1 << WGM12;	// ..
	
	TCCR1B |= 1 << CS12;	// Set prescaler to 256

	ICR1 = F_CPU / 256; 	// Set frequency to 1Hz
	OCR1A = ICR1 / 10;		// Define duty cycle to 10%
	while (1)
		;
}
