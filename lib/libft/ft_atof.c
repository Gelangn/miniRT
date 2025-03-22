/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atof.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:32:47 by anavas-g          #+#    #+#             */
/*   Updated: 2025/03/21 18:41:41 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

float	ft_atof(const char *str)
{
	float result = 0.0;
	float factor = 1.0;
	int decimal_point_seen = 0;
	int sign = 1;

	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r' ||
		*str == '\v' || *str == '\f')
		str++;

	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;

	while (*str)
	{
		if (ft_isdigit(*str))
		{
			if (decimal_point_seen)
			{
				factor /= 10.0;
				result = result + (*str - '0') * factor;
			}
			else
				result = result * 10.0 + (*str - '0');
		}
		else if (*str == '.')
		{
			if (decimal_point_seen)
				break;
			decimal_point_seen = 1;
		}
		else
			break;
		str++;
	}
	return result * sign;
}
/* #include <stdio.h> // printf
int main(void)
{
	char *str = "\t\t0.5";
	printf("%f\n", ft_atof(str));
	return 0;
} */
