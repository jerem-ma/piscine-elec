/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:23:35 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/23 15:36:12 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H
# define UART_H

void	uart_init(void);
int		my_round(double nbr);
char	uart_rx(void);
uint8_t	uart_getline(uint8_t *line, uint8_t size);
void	uart_delchar();
void	uart_printstr(char const *str);
void	uart_tx(unsigned char c);
void	uart_print_hex(unsigned char c);
void	uart_putnbr2(int32_t nbr);
void	uart_putnbr2_double2(double nbr);

#endif
