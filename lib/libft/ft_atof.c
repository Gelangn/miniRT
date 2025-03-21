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

float ft_atof(const char *str)
{
	float result = 0.0;
	float factor = 1.0;
	int decimal_point_seen = 0;

	if (*str == '-')
	{
		factor = -1.0;
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
				return result * factor;
			decimal_point_seen = 1;
		}
		else
			break;
		str++;
	}
	return result * factor;
}
