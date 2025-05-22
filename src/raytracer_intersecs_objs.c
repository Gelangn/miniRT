/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_isecs_objs.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:11:59 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/22 20:48:46 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/**
 * Check intersections between current ray and all spheres
 * Updates closest_isec if a closer intersection is found
 * @param global Structure containing scene and current ray information
 * @param closest_isec Pointer to store the closest intersection found
 */
void	check_sp_isecs(t_global *global, t_intersec *closest_isec)
{
	int			i;
	t_intersec	temp_isec;

	i = -1;
	while (++i < global->scene.num_sp)
	{
		temp_isec = col_sp(global, i);
		if (temp_isec.dist < closest_isec->dist)
		{
			*closest_isec = temp_isec;
			closest_isec->obj_index = i;
			closest_isec->obj_type = 0;
		}
	}
}

/**
 * Check intersections between current ray and all planes
 * Updates closest_isec if a closer intersection is found
 * @param global Structure containing scene and current ray information
 * @param closest_isec Pointer to store the closest intersection found
 */
void	check_pl_isecs(t_global *global, t_intersec *closest_isec)
{
	int			i;
	t_intersec	temp_isec;

	i = -1;
	while (++i < global->scene.num_pl)
	{
		temp_isec = col_pl(global, i);
		if (temp_isec.dist < closest_isec->dist)
		{
			*closest_isec = temp_isec;
			closest_isec->obj_index = i;
			closest_isec->obj_type = 1;
		}
	}
}

/**
 * Check intersections between current ray and all cylinders
 * Updates closest_isec if a closer intersection is found
 * @param global Structure containing scene and current ray information
 * @param closest_isec Pointer to store the closest intersection found
 */
void	check_cy_isecs(t_global *global, t_intersec *closest_isec)
{
	int			i;
	t_intersec	temp_isec;

	i = -1;
	while (++i < global->scene.num_cy)
	{
		temp_isec = col_cy(global, i);
		if (temp_isec.dist < closest_isec->dist)
		{
			*closest_isec = temp_isec;
			closest_isec->obj_index = i;
			closest_isec->obj_type = 2;
		}
	}
}
