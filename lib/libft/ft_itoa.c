/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 18:04:14 by anavas-g          #+#    #+#             */
/*   Updated: 2023/11/06 18:04:18 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	n_len(long nb)
{
	int	len;

	len = 1;
	if (nb < 0)
	{
		nb *= -1;
		len++;
	}
	while (nb > 9)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

static char	*ft_alocate(long number, int len, char *str)
{
	str[len] = '\0';
	len --;
	while (number > 0)
	{
		str[len] = '0' + (number % 10);
		number = number / 10;
		len--;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	char	*chain;
	long	num;
	int		len;
	char	*aux;

	num = n;
	len = n_len(num);
	if (num == 0)
		return (ft_strdup("0"));
	chain = (char *)malloc((len + 1) * sizeof(char));
	if (!chain)
		return (NULL);
	aux = chain;
	if (num < 0)
	{
		*aux = '-';
		aux++;
		num = num * (-1);
		len = len -1;
	}
	*aux = *(char *)ft_alocate(num, len, aux);
	if (n < 0)
		aux--;
	return (chain);
}

// -2147483648 al 2147483647
/* int	main(void)
{
	int s = 10;
	char *str = ft_itoa(s);
	if (str != NULL)
	{
		printf("El número en texto es %s\n", str);
		free(str);
	} else {
		printf("Error de asignación de memoria\n");
	}
	return 0;
} */
