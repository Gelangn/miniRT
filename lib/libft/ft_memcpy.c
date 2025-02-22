/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 19:06:11 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/19 22:39:49 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char	*dest1;
	char	*src1;
	size_t	i;

	dest1 = (char *)dest;
	src1 = (char *)src;
	i = 0;
	if ((dest1 == NULL && src1 == NULL) && n > 0)
		return (NULL);
	while (i < n)
	{
		dest1[i] = src1[i];
		i++;
	}
	return (dest);
}

/* int	main(void)
{
	char	*src ;
	char	dest[10];

	src = "Hola";
	printf("Palabra original: %s\n", src);
	ft_memcpy(dest, src, 3);
	printf("Palabra final: %s\n", dest);
	return (0);
} */
