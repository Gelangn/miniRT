/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 20:17:50 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/15 11:51:25 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*chain;
	char	*aux;

	aux = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!s1 || !s2)
		return (NULL);
	if (!aux)
		return (NULL);
	chain = aux;
	while (*s1 != '\0')
	{
		*aux = *(char *)s1;
		aux++;
		s1++;
	}
	while (*s2 != '\0')
	{
		*aux = *(char *)s2;
		aux++;
		s2++;
	}
	*aux = '\0';
	return (chain);
}

/* int	main(void)
{
	const char	*src1 = "mi texto";
	const char	*src2 = " sagrado";
	char		*result;

	result = ft_strjoin(src1, src2);
	printf("Cadena copiada: %s\n", result);
	return (0);
} */
