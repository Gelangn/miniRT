/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/24 22:17:31 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/02 21:58:37 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return (0);
	while (*s1 && *s2 && n != 0)
	{
		if (*s1 != *s2)
			return ((unsigned char)*s1 - (unsigned char)*s2);
		n--;
		s1++;
		s2++;
	}
	if (n != 0)
		return ((unsigned char)*s1 - (unsigned char)*s2);
	return (0);
}

/* int	main(void)
{
	char	src1[10] = "1234";
	char	src2[13] = "1235";
	int		num;

//en este caso compara los 5 primeros caracteres, 
//si son iguales devuelve un 0, si no, devolvera
	num = 4;
	printf("Cadena comparada: %d\n", ft_strncmp(src1, src2, num));
	printf("Cadena comparada: %d\n", strncmp(src1, src2, num));
	return (0);
} */
