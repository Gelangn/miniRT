/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 16:07:11 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/17 17:56:22 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *str, int c, size_t len)
{
	unsigned char	*temp;

	temp = (unsigned char *)str;
	while (len > 0)
	{
		*temp++ = (unsigned char)c;
		len--;
	}
	return (str);
}

/* int	main(void)
{
	unsigned int	c;
	unsigned int	len;
	unsigned char	strt[12];

	c = 1;
	len = 10;
	ft_memset(strt, c, len);
	printf("%s\n", strt);
	return (0);
}
 */