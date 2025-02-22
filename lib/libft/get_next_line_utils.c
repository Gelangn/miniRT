/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:12:53 by anavas-g          #+#    #+#             */
/*   Updated: 2024/04/13 00:28:33 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strdup_gnl(const char *s)
{
	size_t	len;
	char	*dup;

	len = get_char_len(s, '\0');
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

static char	*ft_alocate(char const *s, unsigned int start, size_t len)
{
	char	*aux;

	if (len < (unsigned long)get_char_len(s, '\0') - start)
		aux = (char *)malloc(sizeof(char) * (len + 1));
	else
		aux = (char *)malloc(sizeof(char) * \
		((unsigned long)get_char_len(s, '\0') - start + 1));
	return (aux);
}

char	*ft_substr_gnl(char const *s, unsigned int start, size_t len)
{
	char	*chain;
	char	*aux;

	if (!s)
		return (NULL);
	if ((unsigned int)get_char_len(s, '\0') < start)
		return (ft_strdup_gnl(""));
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

char	*ft_strchr_gnl(const char *s, int c)
{
	int	i;

	i = 0;
	if (!s)
		return (NULL);
	while (c > 127)
		c -= 128;
	if (c == 0)
	{
		if (*s == '\0')
			return ((char *)&s[0]);
		else
			return ((char *)&s[get_char_len(s, '\0')]);
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

int	get_char_len(const char *chain, char chr)
{
	int		i;

	i = 0;
	if (chain == NULL)
		return (0);
	while (chain[i] && chain[i] != chr)
		i++;
	return (i);
}
