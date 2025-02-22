/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 18:10:12 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/20 19:52:46 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_tolower(int c)
{
	if (c >= 'A' && c <= 'Z')
	{
	c = c + 32;
	}
	return (c);
}

/* int	main(void)
{
	int		str;
	char	to_print;

	str = 'B';
	to_print = (char)ft_tolower(str);
	write(1, &to_print, 1);
} */
