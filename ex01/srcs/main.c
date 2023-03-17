/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/17 14:19:50 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/common.h>
#include <avr/io.h>

int	main()
{
	DDRB = 1 << PINB0;			// PB0 is now output mode. We use DDRB, because B0 is set in this register (cf. letter)
	while (1)
	{
		PORTB = 1 << PINB0;		// PB0 is now set to HIGH. We use PORTB because B0 is set in this register (cf. letter)
	}
}
