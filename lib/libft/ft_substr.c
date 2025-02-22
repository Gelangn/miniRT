/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/10 19:33:38 by anavas-g          #+#    #+#             */
/*   Updated: 2024/04/13 00:49:13 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_alocate(char const *s, unsigned int start, size_t len)
{
	char	*aux;

	if (len < (unsigned long)ft_strlen(s) - start)
		aux = (char *)malloc(sizeof(char) * (len + 1));
	else
		aux = (char *)malloc(sizeof(char) * \
		((unsigned long)ft_strlen(s) - start + 1));
	return (aux);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*chain;
	char	*aux;

	if (!s)
		return (NULL);
	if ((unsigned int)ft_strlen(s) < start)
		return (ft_strdup(""));
	aux = ft_alocate(s, start, len);
	if (!aux)
		return (NULL);
	chain = aux;
	s = s + start;
	while (*s && len != 0)
	{
		*aux = *(char *)s;
		if (len != 0)
		{
			len--;
			aux++;
		}
		s++;
	}
	*aux = '\0';
	return (chain);
}
/* # include <stdio.h>
# include <string.h>
int	main(void)
{
	const char			*src1 = "i just want this part #############";
	char				*result;
	long				comienzo;
	unsigned long long	longitud;
	
	comienzo = 5;
	longitud = 10;
	result = ft_substr(src1, comienzo, longitud);
	printf("Cadena copiada: %s\n", result);
	return (0);
} */
