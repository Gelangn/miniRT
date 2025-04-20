/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:54:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 20:55:12 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_vector	get_sp_normal(t_global *global, int sp_id, t_vector point)
{
	t_sphere	sphere;

	sphere = global->scene.spheres[sp_id];
	return (normalize(subtract(point, sphere.center)));
}

t_vector	get_pl_normal(t_global *global, int pl_id)
{
	if (pl_id < 0 || pl_id >= global->scene.num_pl)
		return ((t_vector){0, 1, 0});
	return (normalize(global->scene.planes[pl_id].normal));
}

t_vector	get_cy_normal(t_global *global, t_intersec intersec)
{
	t_cylinder	cylinder;
	t_vector	axis;
	float		hit_height;
	t_vector	center_at_height;

	cylinder = global->scene.cylinders[intersec.obj_index];
	axis = normalize(cylinder.orientation);
	hit_height = dot(subtract(intersec.point, cylinder.base), axis);
	if (comp_floats(hit_height, 0))
		return (multiply(axis, -1));
	else if (comp_floats(hit_height, cylinder.height))
		return (axis);
	else
	{
		center_at_height = add(cylinder.base, multiply(axis, hit_height));
		return (normalize(subtract(intersec.point, center_at_height)));
	}
}

t_vector	get_surface_normal(t_global *global, t_intersec intersec)
{
	int	obj_type;
	int	obj_index;

	obj_type = intersec.obj_type;
	obj_index = intersec.obj_index;
	if (obj_type < 0 || obj_index < 0 || (obj_type == 0
			&& obj_index >= global->scene.num_sp) || (obj_type == 1
			&& obj_index >= global->scene.num_pl) || (obj_type == 2
			&& obj_index >= global->scene.num_cy))
		return ((t_vector){0, 1, 0});
	if (obj_type == 0)
		return (get_sp_normal(global, obj_index, intersec.point));
	else if (obj_type == 1)
		return (get_pl_normal(global, obj_index));
	else
		return (get_cy_normal(global, intersec));
}
