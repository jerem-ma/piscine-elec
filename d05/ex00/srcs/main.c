/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:58:54 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/23 12:41:34 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

enum eeprom_status	read_from_eeprom(uint16_t addr, uint8_t *data);
enum eeprom_status	write_to_eeprom(uint16_t addr, uint8_t data);

enum eeprom_status
{
	EEPROM_OK,
	EEPROM_INVALID_ADDRESS
};

int	main()
{
	enum eeprom_status	status;
	uint8_t				data;

	uart_init();

	status = write_to_eeprom(0, 0);
	if (status != EEPROM_OK)
	{
		uart_printstr("An error occured while writing counter to 0\n\r");
		return (1);
	}

	uint8_t	value;
	int	button1_value = 0;
	int	switched1 = 0;

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
		{
			read_from_eeprom(0, &value);
			value = (value + 1) % 16;
			write_to_eeprom(0, value);
		}

		read_from_eeprom(0, &value);
		PORTB = ((value & 1) << PORTB0) | (((value & 2) >> 1) << PORTB1) | (((value & 4) >> 2) << PORTB2) | (((value & 8) >> 3) << PORTB4);
	}
}

enum eeprom_status	write_to_eeprom(uint16_t addr, uint8_t data)
{
	if (addr >= 1024)
		return EEPROM_INVALID_ADDRESS;

	uint8_t current_data;

	enum eeprom_status status = read_from_eeprom(addr, &current_data);
	if (status != EEPROM_OK)
		return status;
	if (current_data == data)
		return EEPROM_OK;

	/* Wait for completion of previous write */
	while (EECR & (1 << EEPE))
		;
	/* Set up address and Data Registers */
	EEAR = addr;
	EEDR = data;
	/* Write logical one to EEMPE */
	EECR |= (1 << EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1 << EEPE);
}

enum eeprom_status	read_from_eeprom(uint16_t addr, uint8_t *data)
{
	if (addr >= 1024)
		return EEPROM_INVALID_ADDRESS;

	// p35
	/* Wait for completion of previous write */
	while (EECR & (1 << EEPE))
		;
	/* Set up address register */
	EEAR = addr;
	/* Start eeprom read by writing EERE */
	EECR |= (1 << EERE);
	/* Return data from Data Register */
	*data = EEDR;
	return EEPROM_OK;
}
