/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:00:54 by anavas-g          #+#    #+#             */
/*   Updated: 2024/04/13 00:22:06 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*ft_strjoin_gnl(char const *s1, char const *s2)
{
	char	*chain;
	char	*aux;

	if (!s2)
		return (NULL);
	chain = (char *)malloc((get_char_len(s1, '\0') \
	+ get_char_len(s2, '\0') + 1) * sizeof(char));
	if (!chain)
		return (NULL);
	aux = chain;
	while (s1 && *s1 != '\0')
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

static char	*read_line(char *buffer, char *box, int fd)
{
	char	*aux;
	ssize_t	bytes;

	while (ft_strchr_gnl(box, '\n') == NULL)
	{
		bytes = read(fd, buffer, BUFFER_SIZE);
		if (bytes == -1 || (bytes == 0 && get_char_len(box, '\0') == 0))
		{
			free(box);
			return (NULL);
		}
		buffer[bytes] = '\0';
		if (bytes == 0)
			break ;
		aux = box;
		box = ft_strjoin_gnl(box, buffer);
		if (aux)
			free(aux);
	}
	return (box);
}

static char	*line_control(char **box)
{
	char		*line;
	int			len;
	int			len2;
	char		*aux;

	len = get_char_len(*box, '\n');
	line = ft_substr_gnl(*box, 0, len + 1);
	if (ft_strchr_gnl(line, '\n'))
		len2 = get_char_len(*box + len + 1, '\0');
	else
		len2 = get_char_len(*box + len, '\0');
	aux = *box;
	*box = ft_substr_gnl(*box, len + 1, len2);
	if (aux)
		free(aux);
	if (!**box)
	{
		free(*box);
		*box = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*box;
	char		*buffer;
	char		*line;

	if (BUFFER_SIZE < 0 || fd < 0 || fd > 1024)
		return (NULL);
	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	box = read_line(buffer, box, fd);
	if (!box)
	{
		free(buffer);
		return (NULL);
	}
	free(buffer);
	line = line_control(&box);
	return (line);
}
