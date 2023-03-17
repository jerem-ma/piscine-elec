/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/17 17:23:21 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>

int	main()
{
	DDRB |= 1 << DDB0;			// PB0 is now output mode. We use DDRB, because B0 is set in this register (cf. letter)
	for (;;)
	{
		PORTB = (PORTB & ~(1 << PORTB0)) | ((~PIND & (1 << PIND2)) >> PIND2 << PORTB0); // Empty PB0, get value of PD2, set it to PB0
	}
}
