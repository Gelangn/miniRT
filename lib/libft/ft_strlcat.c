/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/19 19:07:29 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/23 20:42:46 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dstlen;
	size_t	srclen;

	srclen = ft_strlen(src);
	dstlen = ft_strlen(dst);
	if (dstsize <= dstlen)
		return (srclen + dstsize);
	dst = dst + dstlen;
	dstsize = dstsize - dstlen;
	while (1 < dstsize && *src)
	{
		*dst = *src;
		dst++;
		src++;
		dstsize--;
	}
	*dst = '\0';
	return (srclen + dstlen);
}

/* int	main(void)
{
	char		dst1[20] = "Mi_";
	const char	src1[] = "mundo";
	size_t		result;

	result = ft_strlcat(dst1, src1, 25);
	printf("%i Cadena concatenada: %s\n", (int) result, dst1);
} */
