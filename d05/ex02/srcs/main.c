/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:58:54 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/23 13:07:11 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "uart.h"

#define N_COUNTER 4
#define MAGIC 0x42

enum eeprom_status	read_from_eeprom(uint16_t addr, uint8_t *data);
enum eeprom_status	write_to_eeprom(uint16_t addr, uint8_t data);
enum eeprom_status	safe_eeprom_read(void *buffer, size_t offset, size_t length);
enum eeprom_status	safe_eeprom_write(void * buffer, size_t offset, size_t length);

enum eeprom_status
{
	EEPROM_OK,
	EEPROM_INVALID_ADDRESS,
	EEPROM_UNINITIALIZED
};

struct amazing_struct
{
	uint8_t	age;
	uint8_t random_data;
};

int	main()
{
	enum eeprom_status	status;
	uint8_t				data;

	uart_init();

	struct amazing_struct my_struct;

	status = safe_eeprom_read(&my_struct, 0, sizeof(my_struct));
	switch (status)
	{
		case EEPROM_OK:
			uart_printstr("EEPROM_OK\n\r");
			break;
		case EEPROM_INVALID_ADDRESS:
			uart_printstr("EEPROM_INVALID_ADDRESS\n\r");
			break;
		case EEPROM_UNINITIALIZED:
			uart_printstr("EEPROM_UNINITIALIZED\n\r");
			break;
	}
	my_struct = (struct amazing_struct) {.age = 8, .random_data = 0x42};
	status = safe_eeprom_write(&my_struct, 0, sizeof(my_struct));
	switch (status)
	{
		case EEPROM_OK:
			uart_printstr("EEPROM_OK\n\r");
			break;
		case EEPROM_INVALID_ADDRESS:
			uart_printstr("EEPROM_INVALID_ADDRESS\n\r");
			break;
		case EEPROM_UNINITIALIZED:
			uart_printstr("EEPROM_UNINITIALIZED\n\r");
			break;
	}
	my_struct = (struct amazing_struct) {.age = 0, .random_data = 0};
	status = safe_eeprom_read(&my_struct, 0, sizeof(my_struct));
	switch (status)
	{
		case EEPROM_OK:
			uart_printstr("EEPROM_OK\n\r");
			break;
		case EEPROM_INVALID_ADDRESS:
			uart_printstr("EEPROM_INVALID_ADDRESS\n\r");
			break;
		case EEPROM_UNINITIALIZED:
			uart_printstr("EEPROM_UNINITIALIZED\n\r");
			break;
	}
	uart_print_hex(my_struct.age);
	uart_tx(' ');
	uart_print_hex(my_struct.random_data);
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

enum eeprom_status	safe_eeprom_read(void *buffer, size_t offset, size_t length)
{
	uint8_t				data;
	enum eeprom_status	status;

	status = read_from_eeprom(offset, &data);
	if (status != EEPROM_OK)
		return status;
	if (data != MAGIC)
		return EEPROM_UNINITIALIZED;
	offset++;
	for (size_t i = 0; i < length; i++)
	{
		status = read_from_eeprom(offset + i, buffer + i);
		if (status != EEPROM_OK)
			return status;
	}
	return EEPROM_OK;
}

enum eeprom_status	safe_eeprom_write(void *buffer, size_t offset, size_t length)
{
	uint8_t				data;
	uint8_t				*byte_buffer;
	enum eeprom_status	status;

	byte_buffer = (uint8_t *) buffer;
	status = write_to_eeprom(offset, MAGIC);
	if (status != EEPROM_OK)
		return status;
	offset++;
	for (size_t i = 0; i < length; i++)
	{
		status = write_to_eeprom(offset + i, byte_buffer[i]);
		if (status != EEPROM_OK)
			return status;
	}
	return EEPROM_OK;
}
