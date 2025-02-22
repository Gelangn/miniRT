/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 21:10:35 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/10 19:08:21 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	len;
	char	*dup;

	len = ft_strlen(s);
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (0);
	while (*s != '\0')
	{
	*dup = *s;
	s++;
	dup++;
	}
	*dup = '\0';
	dup = dup - len;
	return (dup);
}

/* int	main(void)
{
	const char	*original = "Hello, World!";
	char		*duplicate;

	// Duplicar la cadena original utilizando strdup
	duplicate = ft_strdup(original);
	// Imprimir la cadena duplicada
	printf("Cadena duplicada: %s xyz\n", duplicate);
	return (0);
} */
