/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:57:09 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/26 13:31:52 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	cal_shadow(t_global *global)
{
	t_vector	shadow_origin;
	t_ray_state	state;
	t_intersec	shadow_isec;
	float		transparency;

	shadow_origin = add(global->c_ray.hit.point, multiply(global->c_ray.normal,
				0.005f));
	state = save_ray_state(global);
	global->c_ray.origin = shadow_origin;
	global->c_ray.dir = global->c_light.dir;
	shadow_isec = find_closest_isec(global);
	restore_ray_state(global, state);
	if (shadow_isec.obj_type >= 0 && shadow_isec.dist < global->c_light.distance
		&& !(shadow_isec.obj_type == global->c_ray.hit.obj_type
			&& shadow_isec.obj_index == global->c_ray.hit.obj_index))
	{
		transparency = get_object_transparency(global, shadow_isec);
		if (transparency < 0.01f)
			return (1.0f);
		return (1.0f - transparency);
	}
	return (0.0f);
}
