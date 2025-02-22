/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 20:04:59 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/28 19:14:59 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	i;

	i = (int)ft_strlen(s);
	while (c > 127)
		c = c -128;
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
	while (i >= 0)
	{
		if (c == (int)s[i])
			return ((char *)&s[i]);
		i--;
	}
	return (NULL);
}

/* int	main(void)
{
	const char	*src1 = "mi textox";
	int			num;
	char		*result;

	num = 'x';
	result = ft_strrchr(src1, num);
	printf("Cadena copiada: %s\n", result);
	return (0);
}
 */