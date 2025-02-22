/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdigit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 19:57:25 by anavas-g          #+#    #+#             */
/*   Updated: 2023/09/23 01:17:32 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_isdigit(int c)
{
	if (((c >= '0') && (c <= '9')))
		return (1);
	return (0);
}

/* int	main(void)
{
	char	*str1;
	int		resultado1;

	str1 = "23345";
	resultado1 = ft_isdigit(str1);
	printf("%s: %d\n", str1, resultado1);
	return (0);
} */
