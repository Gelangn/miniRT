/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 18:03:43 by anavas-g          #+#    #+#             */
/*   Updated: 2023/11/06 20:20:58 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_printf_aux(const char fmt, va_list args)
{
	int	len;

	len = 0;
	if (fmt == 'c')
		len = ft_printf_char(va_arg(args, int));
	if (fmt == 's')
		len = ft_printf_str(va_arg(args, char *));
	if (fmt == 'p')
		len = ft_printf_ptr(va_arg(args, long));
	if (fmt == 'd')
		len = ft_printf_nbr(va_arg(args, int));
	if (fmt == 'i')
		len = ft_printf_nbr(va_arg(args, int));
	if (fmt == 'u')
		len = ft_printf_unsigned(va_arg(args, unsigned int));
	if (fmt == 'x')
		len = ft_printf_hex(va_arg(args, unsigned int), 'x');
	if (fmt == 'X')
		len = ft_printf_hex(va_arg(args, unsigned int), 'X');
	if (fmt == '%')
		len = ft_print_percent();
	return (len);
}

int	ft_printf(char const *fmt, ...)
{
	va_list	args;
	int		len;

	if (write(1, "", 0) == -1)
		return (-1);
	len = 0;
	va_start(args, fmt);
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			if (*fmt == '%')
				len += ft_print_percent();
			else
				len += ft_printf_aux(*fmt, args);
		}
		else
			len += ft_printf_char(*fmt);
		fmt++;
	}
	va_end(args);
	return (len);
}
