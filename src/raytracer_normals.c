/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_normals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:54:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 23:12:24 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_vector	get_sp_normal(t_global *global, int sp_id, t_vector point)
{
	t_sphere	sphere;

	sphere = global->scene.spheres[sp_id];
	return (norm(subtract(point, sphere.center)));
}

t_vector	get_pl_normal(t_global *global, int pl_id)
{
	if (pl_id < 0 || pl_id >= global->scene.num_pl)
		return ((t_vector){0, 1, 0});
	return (norm(global->scene.planes[pl_id].normal));
}

t_vector	get_cy_normal(t_global *global, t_intersec isec)
{
    t_cylinder	cyl;
    t_vector	axis;
    float		hit_h;
    t_vector	ctr_h;

    cyl = global->scene.cyls[isec.obj_index];
    axis = norm(cyl.axis);  // Changed from cyl.orientation to cyl.axis
    hit_h = dot(subtract(isec.point, cyl.base), axis);
    if (comp_floats(hit_h, 0))
        return (multiply(axis, -1));
    else if (comp_floats(hit_h, cyl.height))
        return (axis);
    else
    {
        ctr_h = add(cyl.base, multiply(axis, hit_h));
        return (norm(subtract(isec.point, ctr_h)));
    }
}

/**
 * Calculates appropriate surface normal for current ray intersection
 * Handles cases where ray is inside a transparent object
 */
t_vector get_surface_normal(t_global *global, t_intersec isec)
{
    t_vector normal;
    int inside;
    
    isec = global->c_ray.hit;
    
    // Determinar si el rayo está dentro del objeto
    inside = is_inside_object(global, isec, global->c_ray.origin);
    
    // Calcular la normal normalmente
    if (isec.obj_type == 0)
        normal = get_sp_normal(global, isec.obj_index, isec.point);
    else if (isec.obj_type == 1)
        normal = get_pl_normal(global, isec.obj_index);
    else if (isec.obj_type == 2)
        normal = get_cy_normal(global, isec);
    else
        normal = (t_vector){0, 1, 0};
    
    // Si estamos dentro, invertir la normal
    if (inside)
        normal = multiply(normal, -1.0f);
    
    return (normal);
}

