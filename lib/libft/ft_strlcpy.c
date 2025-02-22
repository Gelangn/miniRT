/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/18 20:45:45 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/20 00:56:02 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t		len;
	size_t		i;

	len = ft_strlen(src);
	i = 0;
	if (dstsize == 0)
	{
		return (len);
	}
	while (i < dstsize - 1 && src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}

/* int	main(void)
{
	char		dst1[5];
	const char	*src1 = "mi texto";
	size_t		result;
	//tamaño de la cadena a copiar dstsize
	result = ft_strlcpy(dst1, src1, sizeof(dst1));
	if (result >= sizeof(dst1))
	{
		printf("ft_strlcpy ha truncado la cadena. Destino demasiado pequeño.\n");
	}
	else
	{
		printf("Cadena copiada: %s\n", dst1);
	}
} */
