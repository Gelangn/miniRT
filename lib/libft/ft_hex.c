/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hex.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:45:34 by anavas-g          #+#    #+#             */
/*   Updated: 2023/12/12 20:38:57 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_ptr(unsigned long long nbr)
{
	int	len;

	len = 2;
	ft_putstr_fd("0x", 1);
	len += ft_printf_hex(nbr, 'x');
	return (len);
}

int	ft_printf_hex(unsigned long nbr, char fmt)
{
	int	len;

	len = 1;
	if (nbr >= 16)
		len += ft_printf_hex(nbr / 16, fmt);
	if (fmt == 'x')
		ft_putchar_fd("0123456789abcdef"[nbr % 16], 1);
	if (fmt == 'X')
		ft_putchar_fd("0123456789ABCDEF"[nbr % 16], 1);
	return (len++);
}
