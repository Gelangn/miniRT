/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 13:03:35 by anavas-g          #+#    #+#             */
/*   Updated: 2024/03/28 20:55:28 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <unistd.h>
# include <stdlib.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000000
# endif

char		*get_next_line(int fd);
char		*ft_strdup_gnl(const char *s);
char		*ft_substr_gnl(char const *s, unsigned int start, size_t len);
char		*ft_strchr_gnl(const char *s, int c);
int			get_char_len(const char *chain, char chr);

#endif
