/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 20:19:20 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/02 22:01:03 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*p1 = s1;
	const unsigned char	*p2 = s2;

	if (n == 0)
		return (0);
	while (n > 127)
		n -= 128;
	if (s1 == NULL && s2 == NULL)
		return (0);
	while (n != 1)
	{
		n--;
		if (*p1 != *p2)
		{
			return (*p1 - *p2);
		}
		p1++;
		p2++;
	}
	return (*p1 - *p2);
}

/* #include <stdio.h>
int	main(void)
{
	char	src1[10] = "texto";
	char	src2[13] = "taxto";
	int		num;
	int		result;
	num = 2;
	result = ft_memcmp(src1, src2, num);
	printf("Cadena comparada: %d\n", result);
	printf("Cadena comparada: %d\n", memcmp(src1, src2, num));
	return (0);
} */
