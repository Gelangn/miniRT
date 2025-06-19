/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_properties.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:30:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 13:55:10 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Gets the transparency value for an intersected object
 * Returns 0.0 for opaque objects, 1.0 for fully transparent
 */
float	get_object_transparency(t_global *global, t_intersec isec)
{
	if (isec.obj_type == 0 && isec.obj_index < global->scene.num_sp)
		return (global->scene.spheres[isec.obj_index].transparency);
	else if (isec.obj_type == 1 && isec.obj_index < global->scene.num_pl)
		return (global->scene.planes[isec.obj_index].transparency);
	else if (isec.obj_type == 2 && isec.obj_index < global->scene.num_cy)
		return (global->scene.cyls[isec.obj_index].transparency);
	return (0.0f);
}

/**
 * Gets the reflectivity value for an intersected object
 * Returns 0.0 for non-reflective, 1.0 for perfect mirror
 */
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

/**
 * Gets the refractive index for an intersected object
 * Used for calculating refraction when light passes through transparent objects
 */
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