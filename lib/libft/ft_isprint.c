/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isprint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 13:50:49 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/23 01:16:55 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isprint(int c)
{
	if ((c >= 32) && (c <= 126))
		return (1);
	return (0);
}

/* int	main(void)
{
	char	*str1;
	int		resultado1;

	str1 = "I";
	resultado1 = ft_isprint(str1);
	printf("%s: %d\n", str1, resultado1);
	return (0);
} */
