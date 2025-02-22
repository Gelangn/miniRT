/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/11 20:28:21 by anavas-g          #+#    #+#             */
/*   Updated: 2023/10/18 19:52:21 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	if (s && f)
	{
		while (s[i])
		{
			f(i, &s[i]);
			i++;
		}	
	}
}
/* int	main(void)
{
	char *s;
	void (*f)(unsigned int, char*);
	
	s = "texto";
	f = &ft_putchar_fd;
	ft_striteri(s, f);
	return (0);
} */
