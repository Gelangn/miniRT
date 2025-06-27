/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_intersecs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:59:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/27 12:44:20 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_intersec	find_closest_isec(t_global *global)
{
	t_intersec	closest_isec;

	closest_isec = init_isec();
	if (global->scene.num_sp > 0)
		check_obj_isecs(global, &closest_isec, 0);
	if (global->scene.num_pl > 0)
		check_obj_isecs(global, &closest_isec, 1);
	if (global->scene.num_cy > 0)
		check_obj_isecs(global, &closest_isec, 2);
	return (closest_isec);
}

int	is_valid_isec(t_global *global)
{
	t_intersec	isec;

	isec = global->c_ray.hit;
	if ((isec.obj_type < 0) || (isec.obj_index < 0) || (isec.obj_type == 0
			&& isec.obj_index >= global->scene.num_sp) || (isec.obj_type == 1
			&& isec.obj_index >= global->scene.num_pl) || (isec.obj_type == 2
			&& isec.obj_index >= global->scene.num_cy))
	{
		return (0);
	}
	return (1);
}

static int	get_max_objects(t_global *global, int obj_type)
{
	if (obj_type == 0)
		return (global->scene.num_sp);
	else if (obj_type == 1)
		return (global->scene.num_pl);
	else if (obj_type == 2)
		return (global->scene.num_cy);
	return (0);
}

void	check_obj_isecs(t_global *global, t_intersec *closest, int obj_type)
{
	int			i;
	int			max_objs;
	t_intersec	temp;

	max_objs = get_max_objects(global, obj_type);
	i = -1;
	while (++i < max_objs)
	{
		if (obj_type == 0)
			temp = col_sp(global, i);
		else if (obj_type == 1)
			temp = col_pl(global, i);
		else
			temp = col_cy(global, i);
		if (temp.dist < closest->dist)
		{
			*closest = temp;
			closest->obj_index = i;
			closest->obj_type = obj_type;
		}
	}
}
