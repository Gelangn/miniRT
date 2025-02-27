/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/28 16:07:59 by anavas-g          #+#    #+#             */
/*   Updated: 2024/04/28 16:17:35 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 == *s2)
	{
		if (*s1 == '\0')
			return (0);
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

/* #include <stdio.h>
#include <string.h>
int	main(void)
{
	char	src1[10] = "1234567";
	char	src2[13] = "12345";
	
	//si son iguales devuelve un 0, si no, devolvera la diferencia entre los dos
	printf("Cadena comparada: %d\n", ft_strcmp(src1, src2));
	printf("Cadena comparada: %d\n", strcmp(src1, src2));
	return (0);
} */