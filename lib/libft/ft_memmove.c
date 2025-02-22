/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/17 18:05:26 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/19 17:06:52 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*dst_ptr;
	const char	*src_ptr = (const char *)src;

	dst_ptr = (char *)dst;
	if (dst_ptr == NULL && src_ptr == NULL)
		return (NULL);
	if (dst_ptr > src_ptr && dst_ptr < src_ptr + len)
	{
		dst_ptr += len;
		src_ptr += len;
		while (len--)
		{
			*(--dst_ptr) = *(--src_ptr);
		}
	}
	else
	{
		while (len--)
		{
			*dst_ptr++ = *src_ptr++;
		}
	}
	return (dst);
}
/* #include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
int	main(void)
{
	char	dst1[5];
	char	*src1;

	src1 = "Vladyslava";
	ft_memmove(dst1, src1, (strlen(src1) + 1));
	write(1, dst1, strlen(src1));
	return (0);
} */
