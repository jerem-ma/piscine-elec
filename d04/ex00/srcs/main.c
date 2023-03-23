/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 12:13:48 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/22 13:44:16 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/twi.h>
#include "uart.h"

#define SCL_FREQUENCY 100000
#define BITRATE_GENERATOR_DIVISION_FACTOR (F_CPU / SCL_FREQUENCY - 16) / 2
#define SENSOR_ADDRESS 0x38

void	i2c_init();
void	i2c_init(void);
void	i2c_start(void);
void	i2c_stop(void);
void	wait_for_twi_operation();
int		is_status(uint8_t status);
void	print_status();

int	main(void)
{
	uart_init();
	i2c_init();
	i2c_start();
}

void	i2c_init()
{
	TWBR = BITRATE_GENERATOR_DIVISION_FACTOR;
}

void i2c_start(void)
{
	// Cf. p223-225
	TWCR = 1 << TWINT | 1 << TWSTA | 1 << TWEN; // Enable TWI and generate start
	wait_for_twi_operation();
	if (!is_status(TW_START))				// Check if the START condition has been succesfully transmitted
		return ;
	TWDR = TW_WRITE;							// Write into the bus
	TWDR |= SENSOR_ADDRESS << 1;				// Set destination
	TWCR = 1 << TWINT | 1 << TWEN;				// Start transmission of data
	wait_for_twi_operation();
	if (!is_status(TW_MT_SLA_ACK))			// If it was not ok, do err
		return ;
}

void i2c_stop(void)
{
	TWCR = 1 << TWINT | 1 << TWSTO | 1 << TWEN; // Enable TWI and generate start
}

void	wait_for_twi_operation()
{
	while (!(TWCR & 1 << TWINT))
		;
	print_status();
}

int		is_status(uint8_t status)
{
	return ((TWSR & 0xF8) == status);
}

void	print_status()
{
	char		*str_status;
	const char	*letters = "0123456789ABCDEF";
	uint8_t		status = TWSR & 0xF8;

	switch (status)
	{
		case TW_START:
			str_status = "TW_START";
			break;
		case TW_MT_SLA_ACK:
			str_status = "TW_MT_SLA_ACK";
			break;
		case TW_MT_SLA_NACK:
			str_status = "TW_MT_SLA_NACK";
			break;
		default:
			str_status = "UNKNOWN";
			break;
	}
	uart_printstr(str_status);
	uart_printstr("(0x");
	uart_tx(letters[status / 16]);
	uart_tx(letters[status % 16]);
	uart_printstr(") received !\n\r");
}
