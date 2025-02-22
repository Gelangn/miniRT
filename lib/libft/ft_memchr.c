/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/25 19:34:51 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/02 22:18:11 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*p = s;
	unsigned char		uc;

	uc = (unsigned char)c;
	if (n == 0)
		return (NULL);
	while (n > 0)
	{
		if (uc == *p)
			return ((void *)p);
	p++;
	n--;
	}
	return (NULL);
}

/* int	main(void)
{
	const char	str1 = "mi texto";
	int			c;
	size_t		num;

	c = "x";
	printf("el resultado es: %s\n", ft_memchr(str1, c, num));
} */
