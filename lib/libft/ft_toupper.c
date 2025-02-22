/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_toupper.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:09:31 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/20 19:51:26 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_toupper(int c)
{
	if (c >= 'a' && c <= 'z')
	{
	c = c - 32;
	}
	return (c);
}

/* int	main(void)
{
	int		str;
	char	to_print;

	str = 'a';
	to_print = (char)ft_toupper(str);
	write(1, &to_print, 1);
} */
