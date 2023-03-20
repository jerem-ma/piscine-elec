/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/20 14:11:37 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>

int	main()
{
	DDRB |= 1 << PINB1;					// PB1 is now output mode. We use DDRB, because B0 is set in this register (cf. letter)
	TCCR1A = 1 << COM1A0;				// Use CTC mode
	TCCR1B = 1 << WGM12 | 1 << CS12;	// Set prescaler to 256
	OCR1A = F_CPU / 256 / 2;			// Define reset to 1/2 s
	while (1)
		;
}
