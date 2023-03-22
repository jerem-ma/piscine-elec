/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:23:35 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/22 14:39:20 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H
# define UART_H

void	uart_init(void);
int		my_round(double nbr);
char	uart_rx(void);
void	uart_printstr(char const *str);
void	uart_tx(unsigned char c);
void	uart_print_hex(unsigned char c);

#endif
