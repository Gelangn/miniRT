/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 20:01:24 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/28 19:15:56 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (c > 127)
		c -= 128;
	if (c == 0)
	{
		if (*s == '\0')
		{
			return ((char *)&s[0]);
		}
		else
		{
			return ((char *)&s[ft_strlen(s)]);
		}
	}
	if (s == 0)
		return (NULL);
	while (s[i])
	{
		if (c == (int)s[i])
			return ((char *)&s[i]);
	i++;
	}
	return (0);
}

/* int	main(void)
{
	const char	*src1 = "mi texto";
	int			num;
	char		*result;

	num = 'u';
	result = ft_strchr(src1, num);
	printf("Cadena copiada: %s\n", result);
	result = strchr(src1, num);
	printf("Cadena copiada: %s\n", result);
	return (0);
} */
