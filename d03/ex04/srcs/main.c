/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 13:38:55 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/21 20:46:52 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define USERNAME_VALID 0
#define PASSWORD_VALID 1

void	uart_init(void);
int		my_round(double nbr);
char	uart_rx(void);
void	uart_printstr(char const *str);
void	uart_tx(unsigned char c);
int	ask(char *check, char hide_prompt);
int	ft_strlen(char const *str);

int	main()
{
	char	*username = "spectre";
	char	*password = "fantome";
	int		credentials_validity = 0;

	uart_init();
	DDRB = 1 << DDB0 | 1 << DDB1 | 1 << DDB2 | 1 << DDB4;

	uart_printstr("Enter your login:\n\r");
	while (credentials_validity != 3)
	{
		uart_printstr("\tusername: ");
		credentials_validity = ask(username, 0) << USERNAME_VALID;
		uart_printstr("\tpassword: ");
		credentials_validity |= ask(password, 1) << PASSWORD_VALID;
		if (credentials_validity != 3)
			uart_printstr("Bad combinaison username/password\r\n\r\n");
	}
	uart_printstr("Hello ");
	uart_printstr(username);
	uart_printstr("!\r\n");
	uart_printstr("Shall we play a game?\r\n");
	for (int i = 0; i < 6; i++)
	{
		PORTB ^= (1 << PORTB0 | 1 << PORTB1 | 1 << PORTB2 | 1 << PORTB4);
		_delay_ms(500);
	}
}

void	uart_init(void)
{
	UBRR0L = my_round(F_CPU / (16 * UART_BAUDRATE)); // Set baudrate
	UCSR0B = (1 << TXEN0 | 1 << RXEN0); // Enable transmission of data
	// Default is 8N1 frame format (cf. p201)
}

int	my_round(double nbr)
{
	if (nbr - (int) nbr < 0.5)
		return (int) nbr;
	return (int) nbr + 1;
}

void	uart_printstr(char const *str)
{
	while (*str)
	{
		uart_tx(*str);
		str++;
	}
}

void	uart_tx(unsigned char c)
{
	// Wait until transmit buffer empty (cf. p186 and p200)
	while (!(UCSR0A & (1 << UDRE0)))
		;
	UDR0 = c;
}

char	uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0))) // Wait for a character to be sent
		;

	return UDR0;
}

int	ask(char *check, char hide_prompt)
{
	int		valid_until = 0;
	int		typed_index = 0;
	char	c = 0;

	while ((c = uart_rx()) != '\r')
	{
		if (c == '\x7F') // DEL
		{
			if (typed_index != 0)
			{
				uart_printstr("\b \b"); // Move cursor to the left then empty it then move it again
				if (valid_until == typed_index)
					valid_until--;
				typed_index--;
			}
		}
		else
		{
			if (hide_prompt)
				uart_tx('*');
			else
				uart_tx(c);
			if (typed_index == valid_until && check[typed_index] == c)
				valid_until++;
			typed_index++;
		}
	}
	uart_printstr("\r\n");
	return (valid_until == ft_strlen(check));
}

int	ft_strlen(char const *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}
