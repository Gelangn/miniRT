/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_nbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 17:44:53 by anavas-g          #+#    #+#             */
/*   Updated: 2023/11/06 19:46:59 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_char(int c)
{
	ft_putchar_fd(c, 1);
	return (1);
}

int	ft_printf_str(char *str)
{
	if (str == NULL)
	{
		return (ft_printf_str("(null)"));
	}
	if (*str == '\0')
		return (0);
	ft_putstr_fd(str, 1);
	return (ft_strlen(str));
}

int	ft_printf_unsigned(unsigned int nbr)
{
	int	len;	

	len = 1;
	if (nbr >= 10)
		len += ft_printf_unsigned(nbr / 10);
	ft_putchar_fd(nbr % 10 + '0', 1);
	return (len++);
}

int	ft_printf_nbr(int nbr)
{
	int	len;	

	len = 0;
	if (nbr < 0)
	{
		ft_putchar_fd('-', 1);
		nbr = -nbr;
		len = 1;
	}
	len += ft_printf_unsigned(nbr);
	return (len);
}

int	ft_print_percent(void)
{
	ft_printf_char('%');
	return (1);
}
