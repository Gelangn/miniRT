/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/05 00:09:03 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/05 00:43:19 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	k = 0;
	if (little[0] == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = i;
		while (big[j] == little[k] && j < len)
		{
			j++;
			k++;
			if (little[k] == '\0')
				return ((char *)&big[i]);
		}
		k = 0;
		i++;
	}
	return (0);
}

/* int	main(void)
{
	const char	*src1 = "mi texto";
	const char	*src2 = "texto";
	size_t		num;
	char		*result;

	num = 5;
	result = strnstr(src1, src2, num);
	printf("Cadena copiada: %s\n", result);
	result = ft_strnstr(src1, src2, num);
	printf("Cadena copiada: %s\n", result);
	return (0);
} */
