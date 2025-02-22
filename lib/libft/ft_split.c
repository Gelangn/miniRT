/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 20:20:49 by anavas-g          #+#    #+#             */
/*   Updated: 2024/04/13 00:39:22 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free_2d_str(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

static int	ft_count_words(char const *s, char c)
{
	int	count;

	count = 0;
	while (*s != '\0')
	{
		while (*s != '\0' && *s == c)
			s++;
		if (*s != c && *s != '\0')
			count++;
		while (*s != '\0' && *s != c)
			s++;
	}
	return (count);
}

static char	**ft_fill_chain(const char *s, char **chain2, char c)
{
	int	start;
	int	end;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
		{
			start = i;
			while (s[i] != '\0' && s[i] != c)
				i++;
			end = i;
			chain2[j] = ft_substr(s, start, end - start);
			if (!chain2[j])
				return (ft_free_2d_str(chain2), NULL);
			j++;
		}
	}
	chain2[j] = NULL;
	return (chain2);
}

char	**ft_split(char const *s, char c)
{
	char	**chain;
	int		words;

	if (!s)
		return (NULL);
	words = ft_count_words(s, c);
	chain = (char **)malloc(sizeof(char *) * (1 + words));
	if (!chain)
		return (NULL);
	chain = ft_fill_chain(s, chain, c);
	return (chain);
}

/* int	main(void)
{
	const char	*src1;
	char		src2;
	char		**result;

	src1 = "   mi texto  sagrado     ";
	src2 = ' ';
	result = ft_split(src1, src2);
	while (*result != NULL)
	{
		printf("Cadena copiada: \"%s\"\n", *result);
		result++;
	}
	return (0);
} */
