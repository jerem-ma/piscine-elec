/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/17 18:28:15 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */


#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	int	value = 8;
	int	dir = 0;

	DDRB |= 1 << DDB0 | 1 << DDB1 | 1 << DDB2 | 1 << DDB4;
	for (;;)
	{
		PORTB = ((value & 1) << PORTB0) | (((value & 2) >> 1) << PORTB1) | (((value & 4) >> 2) << PORTB2) | (((value & 8) >> 3) << PORTB4);
		if (dir == 0)
			value >>= 1;
		if (dir == 1)
			value <<= 1;
		if (value == 1 || value == 8)
			dir = !dir;
		_delay_ms(50);
	}
}
