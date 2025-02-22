/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 16:53:52 by anavas-g          #+#    #+#             */
/*   Updated: 2024/04/13 17:55:50 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_trim_str(const char *str, char num)
{
	char	*trim;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (str[i] != num && str[i] != '\0')
		i++;
	trim = (char *)malloc(sizeof(char) * (i + 1));
	if (!trim)
		return (NULL);
	while (j < i)
	{
		trim[j] = str[j];
		j++;
	}
	trim[i] = '\0';
	return (trim);
}

/* int	main(void)
{
	const char	*str;
	char		num;
	char		*result;

	num = ',';
	str = "   i just want,this part #############";
	result = ft_trim_str(str, num);
	printf("Cadena recortada: %s\n", result);
	return (0);
}
 */