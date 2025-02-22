/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 20:06:18 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/23 01:17:56 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isalnum(int c)
{
	if (((c >= 'A') && (c <= 'Z')) || \
	((c >= 'a') && (c <= 'z')) || \
	((c >= '0') && (c <= '9')))
		return (1);
	return (0);
}

/* int	main(void)
{
	char	*str1;
	int		resultado1;

	str1 = "78Iloveyou";
	resultado1 = ft_isalnum(str1);
	printf("%s: %d\n", str1, resultado1);
	return (0);
} */
