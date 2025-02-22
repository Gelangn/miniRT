/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 18:03:21 by anavas-g          #+#    #+#             */
/*   Updated: 2024/03/05 20:26:40 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include "../libft/libft.h"
# include <stdarg.h>

int	ft_printf(char const *fmt, ...);
int	ft_printf_hex(unsigned long nbr, char fmt);
int	ft_printf_ptr(unsigned long long nbr);
int	ft_printf_char(int c);
int	ft_printf_str(char *str);
int	ft_printf_unsigned(unsigned int nbr);
int	ft_printf_nbr(int nbr);
int	ft_print_percent(void);

#endif
