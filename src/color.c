/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:13:10 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 21:36:00 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_color	color_scale(t_color color, float factor)
{
	t_color	result;

	result.r = color.r * factor;
	result.g = color.g * factor;
	result.b = color.b * factor;
	return (result);
}

t_color	color_add(t_color c1, t_color c2)
{
	t_color	result;

	result.r = c1.r + c2.r;
	result.g = c1.g + c2.g;
	result.b = c1.b + c2.b;
	return (result);
}

t_color	get_object_color(t_global *global)
{
	t_intersec	isec;

	isec = global->current_intersec;
	if (isec.obj_type == 0)
		return (global->scene.spheres[isec.obj_index].color);
	else if (isec.obj_type == 1)
		return (global->scene.planes[isec.obj_index].color);
	else
		return (global->scene.cylinders[isec.obj_index].color);
}

t_ray_state	save_ray_state(t_global *global)
{
	t_ray_state	state;

	state.origin = global->current_ray_origin;
	state.dir = global->current_ray_dir;
	state.isec = global->current_intersec;
	return (state);
}

void	restore_ray_state(t_global *global, t_ray_state state)
{
	global->current_ray_origin = state.origin;
	global->current_ray_dir = state.dir;
	global->current_intersec = state.isec;
}
