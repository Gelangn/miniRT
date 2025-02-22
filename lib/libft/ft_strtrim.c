/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 20:19:32 by anavas-g          #+#    #+#             */
/*   Updated: 2024/04/13 17:08:33 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	int		i;

	i = 0;
	if (!s1 || !set)
		return (NULL);
	while (s1[i] != '\0' && ft_strchr(set, s1[i]))
		i++;
	if (s1[i] == '\0')
		return (ft_strdup(""));
	start = i;
	i = ft_strlen(s1);
	while (i != 0 && ft_strchr(set, s1[i]))
		i--;
	end = i;
	return (ft_substr(s1, start, (end - start) + 1));
}

/* int	main(void)
{
	const char	*src1 = "        ";
	char		*result;
	const char	*longitud;

	longitud = " ";
	result = ft_strtrim(src1, longitud);
	printf("Cadena copiada: <%s>\n", result);
	return (0);
} */
