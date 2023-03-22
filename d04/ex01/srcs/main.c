/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 12:13:48 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/22 22:18:10 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/twi.h>
#include "uart.h"
#include <util/delay.h>

#define SCL_FREQUENCY 100000
#define BITRATE_GENERATOR_DIVISION_FACTOR (F_CPU / SCL_FREQUENCY - 16) / 2
#define SENSOR_ADDRESS 0x38

enum	i2c_mode
{
	I2C_READ = TW_READ,
	I2C_WRITE = TW_WRITE
};

enum i2c_read_status
{
	READ_OK,
	READ_NOK
};

void	i2c_init();
void	i2c_start(enum i2c_mode);
void	i2c_stop(void);
void	wait_for_twi_operation();
int		is_status(uint8_t status);
void	print_status();
void	i2c_read(uint8_t *data, uint8_t want_more_data);
void	i2c_batch_read(uint8_t *data, uint8_t size);
void	i2c_write(uint8_t data);

int	main(void)
{
	uart_init();

//	uart_printstr("--Initializing i2c...\n\r");
	i2c_init();

	_delay_ms(40);	// Wait for module starting
//	uart_printstr("--Start communication in write mode...\n\r");
//	i2c_start(I2C_WRITE);
//
//	uart_printstr("--Ask for status...\n\r");
//	i2c_write(0x71);
//
//	uart_printstr("--Start communication in read mode...\n\r");
//	i2c_stop();
//	i2c_start(I2C_READ);
//
//	uart_printstr("--Reading status...\n\r");
	uint8_t	ret;
//	i2c_read(&ret, 0);
//	uart_printstr("Here is status: ");
//	uart_print_hex(ret);
//	uart_printstr("\n\r");
//	i2c_stop();

	while (1)
	{

//		uart_printstr("--Trigger measurement...\n\r");
		i2c_start(I2C_WRITE);
		i2c_write(0xAC);
		i2c_write(0x33);
		i2c_write(0x00);
		i2c_stop();

//		uart_printstr("--Read answer\n\r");
		do
		{
			_delay_ms(80);
			i2c_start(I2C_READ);
			i2c_read(&ret, 0);
			i2c_stop();
		} while (ret & (1 << 7));

//		uart_printstr("Read data...\n\r");
		uint8_t	data[7];

		i2c_start(I2C_READ);
		i2c_batch_read(data, 7);
		for (int i = 0; i < 6; i++)
		{
			uart_print_hex(data[i]);
			uart_tx(' ');
		}
		uart_printstr("\n\r");
		i2c_stop();
		_delay_ms(1000);
	}
}

void	i2c_init()
{
	TWBR = BITRATE_GENERATOR_DIVISION_FACTOR;
}

void i2c_start(enum i2c_mode mode)
{
	// Cf. p223-225
	TWCR = 1 << TWINT | 1 << TWSTA | 1 << TWEN; // Enable TWI and generate start
	wait_for_twi_operation();
	if (!is_status(TW_START) && !is_status(TW_REP_START))				// Check if the START condition has been succesfully transmitted
		return ;
//	TWDR = mode;							// Set mode of the bus
	TWDR = mode | SENSOR_ADDRESS << 1;						// Set destination
	TWCR = 1 << TWINT | 1 << TWEN;						// Start transmission of data
	wait_for_twi_operation();
	if (!is_status(TW_MT_SLA_ACK) && !is_status(TW_MR_SLA_ACK))			// If it was not ok, do err
		return ;
}

void i2c_stop(void)
{
	TWCR = 1 << TWINT | 1 << TWSTO | 1 << TWEN; // Enable TWI and generate start
	while (TWCR & 1 << TWSTO)
		;
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
	uint8_t		status = TWSR & 0xF8;

	switch (status)
	{
		case TW_START:
			str_status = "TW_START";
			break;
		case TW_REP_START:
			str_status = "TW_REP_START";
			break;
		case TW_MT_SLA_ACK:
			str_status = "TW_MT_SLA_ACK";
			break;
		case TW_MT_SLA_NACK:
			str_status = "TW_MT_SLA_NACK";
			break;
		case TW_MT_DATA_ACK:
			str_status = "TW_MT_DATA_ACK";
			break;
		case TW_MT_DATA_NACK:
			str_status = "TW_MT_DATA_NACK";
			break;
		case TW_MR_SLA_ACK:
			str_status = "TW_MR_SLA_ACK";
			break;
		case TW_MR_SLA_NACK:
			str_status = "TW_MR_SLA_NACK";
			break;
		case TW_MR_DATA_ACK:
			str_status = "TW_MR_DATA_ACK";
			break;
		case TW_MR_DATA_NACK:
			str_status = "TW_MR_DATA_NACK";
			break;
		default:
			str_status = "UNKNOWN";
			break;
	}
	uart_printstr(str_status);
	uart_tx('(');
	uart_print_hex(status);
	uart_printstr(") received !\n\r");
}

void	i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = 1 << TWINT | 1 << TWEN;			// Start transmission of data
	wait_for_twi_operation();
	if (!is_status(TW_MT_SLA_ACK))			// If it was not ok, do err
		return ;
}

void	i2c_read(uint8_t *data, uint8_t want_more_data)
{
	TWCR = 1 << TWINT | 1 << TWEN | ((want_more_data & 1) << TWEA); // Start transmission of data
	wait_for_twi_operation();
	*data = TWDR;
}

void	i2c_batch_read(uint8_t *data, uint8_t size)
{
	enum i2c_read_status	status;

	for (uint8_t i = 0; i < size; i++)
		i2c_read(data + i, i != size - 1);
}
