/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_properties.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:30:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/25 01:04:24 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Returns the transparency value of the object hit by the ray
 * 0.0 = opaque, 1.0 = fully transparent
 */
float	get_object_transparency(t_global *global, t_intersec isec)
{
	
	// Esferas
    if (isec.obj_type == 0)
    {
		if (isec.obj_index >= 0 && isec.obj_index < global->scene.num_sp)
		return (global->scene.spheres[isec.obj_index].transparency);
    }
    // Planos 
    else if (isec.obj_type == 1)
    {
		if (isec.obj_index >= 0 && isec.obj_index < global->scene.num_pl)
		return (global->scene.planes[isec.obj_index].transparency);
    }
    // Cilindros - SOLUCIÓN DIRECTA
    else if (isec.obj_type == 2)
    {
		//printf("Object type: %d, index: %d\n", isec.obj_type, isec.obj_index);
        
		if (isec.obj_index >= 0 && isec.obj_index < global->scene.num_cy) {
            float transp = global->scene.cyls[isec.obj_index].transparency;
            //printf("CILINDRO TRANSPARENCIA: %.2f\n", transp);
            return transp;
        }
		else
			printf("CILINDRO - indice fuera de rango\n");
	}
	return (0.0f);
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
		return (global->scene.spheres[isec.obj_index].refractive_idx);
	else if (isec.obj_type == 1 && isec.obj_index < global->scene.num_pl)
		return (global->scene.planes[isec.obj_index].refractive_idx);
	else if (isec.obj_type == 2 && isec.obj_index < global->scene.num_cy)
		return (global->scene.cyls[isec.obj_index].refractive_idx);
	return (1.0f);
}