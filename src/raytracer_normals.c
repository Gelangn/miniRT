/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_normals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:54:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 22:18:17 by anavas-g         ###   ########.fr       */
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
	t_cylinder	cylinder;
	t_vector	axis;
	float		hit_h;
	t_vector	ctr_h;

	cylinder = global->scene.cylinders[isec.obj_index];
	axis = norm(cylinder.orientation);
	hit_h = dot(subtract(isec.point, cylinder.base), axis);
	if (comp_floats(hit_h, 0))
		return (multiply(axis, -1));
	else if (comp_floats(hit_h, cylinder.height))
		return (axis);
	else
	{
		ctr_h = add(cylinder.base, multiply(axis, hit_h));
		return (norm(subtract(isec.point, ctr_h)));
	}
}

t_vector	get_surface_normal(t_global *global, t_intersec isec)
{
	int	obj_type;
	int	obj_index;

	obj_type = isec.obj_type;
	obj_index = isec.obj_index;
	if (obj_type < 0 || obj_index < 0 || (obj_type == 0
			&& obj_index >= global->scene.num_sp) || (obj_type == 1
			&& obj_index >= global->scene.num_pl) || (obj_type == 2
			&& obj_index >= global->scene.num_cy))
		return ((t_vector){0, 1, 0});
	if (obj_type == 0)
		return (get_sp_normal(global, obj_index, isec.point));
	else if (obj_type == 1)
		return (get_pl_normal(global, obj_index));
	else
		return (get_cy_normal(global, isec));
}
