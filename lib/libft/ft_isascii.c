/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isascii.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 20:20:56 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/23 01:18:27 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isascii(int c)
{
	if (c >= 0)
	{
		if (c > 127)
			return (0);
		return (1);
	}
	else
	{
		return (0);
	}
	return (0);
}

/* int	main(void)
{
	char	c1;

	//str1 = "h";
	write(1, &str1, 1);
	return (0);
} */
