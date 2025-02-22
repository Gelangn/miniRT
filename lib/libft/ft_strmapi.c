/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 20:27:05 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/26 20:16:23 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	i;
	char			*str;
	char			*aux;

	i = 0;
	aux = 0;
	aux = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!aux)
		return (NULL);
	if (s && f)
	{
		while (s[i])
		{
			aux[i] = f(i, s[i]);
			i++;
		}	
	}
	aux[i] = '\0';
	str = aux;
	return (str);
}
/* # include <stdio.h>
# include <string.h>
char	example_function(unsigned int index, char character)
{
	return (character + index);
}

int	main(void)
{
	const char	*input_string = "Hello, World!";
	char		*result = ft_strmapi(input_string, example_function);

	if (result)
	{
		printf("Original string: %s\n", input_string);
		printf("Mapped string: %s\n", result);
	}
	else
	{
		printf("Memory allocation error.\n");
	}
	return (0);
} */
