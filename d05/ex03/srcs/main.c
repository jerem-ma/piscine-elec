/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 11:58:54 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/23 14:09:37 by jmaia            ###   ###               */
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

enum eeprom_status	eepromalloc_write(uint16_t id, void *buffer, uint16_t length);
enum eeprom_status	eepromalloc_read(uint16_t id, void *buffer, uint16_t length);
enum eeprom_status	eepromalloc_free(uint16_t id);
enum eeprom_status	eepromalloc_init();
enum eeprom_status	eepromalloc_force_init();

enum eeprom_status	batch_read_from_eeprom(uint16_t addr, void *data, uint16_t length);
enum eeprom_status	batch_write_from_eeprom(uint16_t addr, void *data, uint16_t length);

enum eeprom_status
{
	EEPROM_OK = 0,
	EEPROM_INVALID_ADDRESS = 1,
	EEPROM_UNINITIALIZED = 2,
};

struct data_block
{
	uint16_t	key;
	uint16_t	address;
};

int	main()
{
	enum eeprom_status	status;
	uint8_t				data;

	uart_init();
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

enum eeprom_status	batch_read_from_eeprom(uint16_t addr, void *buffer, uint16_t length)
{
	uint8_t				*byte_buffer;
	enum eeprom_status	status;

	byte_buffer = (uint8_t *) buffer;
	for (uint16_t i = 0; i < length; i++)
	{
		status = read_from_eeprom(addr + i, byte_buffer + i);
		if (status != EEPROM_OK)
			return status;
	}
	return EEPROM_OK;
}

enum eeprom_status	batch_write_from_eeprom(uint16_t addr, void *data, uint16_t length)
{
	uint8_t				*byte_data;
	enum eeprom_status	status;

	byte_data = (uint8_t *) data;
	for (uint16_t i = 0; i < length; i++)
	{
		status = write_to_eeprom(addr + i, byte_data[i]);
		if (status != EEPROM_OK)
			return status;
	}
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

// L'emplacement 0 sera notre tableau, et ensuite je partirai de la fin
// Si on free, je vais juste prendre le dernier et le mettre à la place du free pour garder une continuité
//
// Ça va ressembler à ça:
// 0 => Nombre magique pour savoir s'il faut init
// 1-2 => Nombre de trucs alloués (uint16_t)
// Bytes suivants :
// uint16_t clé
// uint16_t addresse
// uint16_t size

// On vient lire les bytes (1-2) pour savoir combien de truc alloué
// On boucle tant qu'on n'a pas dépassé le nombre de trucs alloué, ni rencontré la clé
// Quand on a trouvé la clé, on compare les 2 size
// Si c'est différent, on prend une nouvelle addresse

// On peut tout stocker à la suite mais si on free, on va faire des trous




// 0	=> Magic number
// 1-2	=> Nombre de trucs alloués
// 3-4	=> Taille
// x	=> Data
// Y+1-2 => tAILLE
// Z	=> Data
// ...

enum eeprom_status	eepromalloc_write(uint16_t id, void *buffer, uint16_t length)
{
	
}

enum eeprom_status	eepromalloc_read(uint16_t id, void *buffer, uint16_t length)
{
	
}

enum eeprom_status	eepromalloc_free(uint16_t id)
{
	
}

enum eeprom_status eepromalloc_init()
{
	enum eeprom_status	status;
	uint8_t				data;

	status = read_from_eeprom(0, &data);
	if (status != EEPROM_OK)
		return status;
	if (data != MAGIC)
		return eepromalloc_force_init();
	return EEPROM_OK;
}

enum eeprom_status eepromalloc_force_init()
{
	enum eeprom_status	status;

	status = write_to_eeprom(0, MAGIC);
	status |= write_to_eeprom(1, 0);
	status |= write_to_eeprom(2, 0);
	return status;
}
