/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:57:09 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/23 11:15:16 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	cal_shadow(t_global *global)
{
	t_vector	shadow_origin;
	//t_ray_state	state;
	t_intersec	shadow_isec;

	shadow_origin = add(global->c_ray.hit.point, multiply(global->c_ray.normal,
				0.005f));
	save_ray_state(global);
	global->c_ray.origin = shadow_origin;
	global->c_ray.dir = global->c_light.dir;
	shadow_isec = find_closest_isec(global);
	restore_ray_state(global);
	if (shadow_isec.obj_type >= 0 && shadow_isec.dist < global->c_light.distance
		&& !(shadow_isec.obj_type == global->c_ray.hit.obj_type
			&& shadow_isec.obj_index == global->c_ray.hit.obj_index))
	{
		return (1);
	}
	return (0);
}
