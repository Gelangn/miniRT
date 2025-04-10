/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isometric.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 14:03:17 by anavas-g          #+#    #+#             */
/*   Updated: 2025/03/16 17:45:43 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* void	isometric(t_point *point)
{
	double	angle;

	angle = PI / 6;
	point->scrn_x = (point->point_x - point->point_y) * cos(angle);
	point->scrn_y = (point->point_x + point->point_y) * sin(angle) \
		- point->point_z;
}

void	points_3d_to_2d(t_scene *scene)
{
	int	i;

	i = -1;
	while (++i < scene->nr_elems)
		isometric(&scene->points[i]);
} */
