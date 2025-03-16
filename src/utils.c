/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:24:11 by anavas-g          #+#    #+#             */
/*   Updated: 2025/03/16 17:45:52 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	finish(char *s)
{
	if (errno == 0)
		ft_putendl_fd(s, 2);
	else
		perror(s);
	exit(1);
}

int	valid_point(char *value)
{
	int	valid;

	valid = 0;
	if (*value == '-' || *value == '+' || ft_isdigit(*value))
		valid++;
	value++;
	while (ft_isdigit(*value))
	{
		value++;
		valid++;
	}
	if (valid == 0)
		return (0);
	else
		return (1);
}

/* void	splited(t_scene *scene, int j, int i, int index)
{
	char	**split;

	split = ft_split(scene->lines[j], ' ');
	if (ft_strchr(split[i], ',') != NULL)
	{
		scene->points[index].point_z = ft_atoi(ft_trim_str(split[i], ','));
		scene->points[index].color = \
			ft_atoi_base(ft_strchr(split[i], 'x') + 1, 16);
	}
	else
	{
		if (valid_point(split[i]) == 0)
			finish(ERR_SCENE);
		scene->points[index].point_z = ft_atoi(split[i]);
		scene->points[index].color = DEFAULT_COLOR;
	}
	dbl_free(split);
} */

int	ft_atoi_base(const char *str, int base)
{
	int		neg;
	int		res;
	int		i;

	neg = 1;
	res = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		neg = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && (ft_isdigit(str[i]) || (str[i] >= 'a' && str[i] <= 'f')
			|| (str[i] >= 'A' && str[i] <= 'F')))
	{
		if (ft_isdigit(str[i]))
			res = res * base + str[i] - '0';
		else if (str[i] >= 'a' && str[i] <= 'f')
			res = res * base + str[i] - 'a' + 10;
		else if (str[i] >= 'A' && str[i] <= 'F')
			res = res * base + str[i] - 'A' + 10;
		i++;
	}
	return (res * neg);
}
