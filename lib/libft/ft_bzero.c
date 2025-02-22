/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 21:16:07 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/17 18:00:25 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <string.h>

void	ft_bzero(void *str, size_t n)
{
	size_t			i;

	i = 0;
	while (i < n)
	{
		((unsigned char *)str)[i] = 0;
		i++;
	}
}

/* int	main(void)
{
	char	str[20];

	printf("Original string: %s\n", "Hello, World!");
	ft_bzero(str, 2);
	printf("Final string: %s\n", str);
	return (0);
} */
