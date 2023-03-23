/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jmaia <jmaia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 13:23:35 by jmaia             #+#    #+#             */
/*   Updated: 2023/03/22 13:25:08 by jmaia            ###   ###               */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H
# define UART_H

void	uart_init(void);
int		my_round(double nbr);
char	uart_rx(void);
void	uart_printstr(char const *str);
void	uart_tx(unsigned char c);

#endif
