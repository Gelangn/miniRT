/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_properties.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:30:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 14:03:51 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

float	get_object_transparency(t_global *global, t_intersec isec)
{
	float transparency = 0.0f;
	
	if (isec.obj_type == 0 && isec.obj_index < global->scene.num_sp)
		transparency = global->scene.spheres[isec.obj_index].transparency;
	else if (isec.obj_type == 1 && isec.obj_index < global->scene.num_pl)
		transparency = global->scene.planes[isec.obj_index].transparency;
	else if (isec.obj_type == 2 && isec.obj_index < global->scene.num_cy)
		transparency = global->scene.cyls[isec.obj_index].transparency;
	
	// Debug print occasionally
	static int debug_count = 0;
	if (debug_count++ < 5 && transparency > 0.1f)
		printf("DEBUG: Object type=%d, index=%d, transparency=%.2f\n", 
			   isec.obj_type, isec.obj_index, transparency);
	
	return transparency;
}

float	get_object_reflectivity(t_global *global, t_intersec isec)
{
	if (isec.obj_type == 0 && isec.obj_index < global->scene.num_sp)
		return (global->scene.spheres[isec.obj_index].reflectivity);
	else if (isec.obj_type == 1 && isec.obj_index < global->scene.num_pl)
		return (global->scene.planes[isec.obj_index].reflectivity);
	else if (isec.obj_type == 2 && isec.obj_index < global->scene.num_cy)
		return (global->scene.cyls[isec.obj_index].reflectivity);
	return (0.0f);
}

float	get_object_refractive_index(t_global *global, t_intersec isec)
{
	if (isec.obj_type == 0 && isec.obj_index < global->scene.num_sp)
		return (global->scene.spheres[isec.obj_index].refractive_index);
	else if (isec.obj_type == 1 && isec.obj_index < global->scene.num_pl)
		return (global->scene.planes[isec.obj_index].refractive_index);
	else if (isec.obj_type == 2 && isec.obj_index < global->scene.num_cy)
		return (global->scene.cyls[isec.obj_index].refractive_index);
	return (1.0f);
}