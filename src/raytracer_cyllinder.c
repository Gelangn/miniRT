/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_cyllinder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:32:59 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 21:12:31 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_intersec	cal_lateral_intersec(t_global *global, int cy_id)
{
	t_intersec	intersec;
	float		discriminant;

	intersec = init_intersec();
	init_lateral_intersec_vars(global, cy_id);
	discriminant = cal_lateral_discriminant(global, cy_id);
	if (discriminant < 0)
		return (intersec);
	get_intersec_points(global, dot(global->current_cyl_vars.dir_perp,
			global->current_cyl_vars.dir_perp), 2
		* dot(global->current_cyl_vars.dir_perp,
			global->current_cyl_vars.oc_perp), discriminant);
	intersec = check_lateral_hits(global, cy_id);
	return (intersec);
}

// Función de intersección de tapas simplificada
t_intersec	cal_cap_intersec(t_global *global, int cy_id, int cap_sign)
{
	t_intersec	intersec;
	t_cylinder	*cylinder;
	t_vector	cap_center;
	t_vector	normal;
	float		denom;
	float		t;
	t_vector	hit_point;
	float		dist_from_center;

	intersec = init_intersec();
	cylinder = &global->scene.cylinders[cy_id];
	cap_center = get_cap_center(cylinder, cap_sign);
	normal = get_cap_normal(cylinder, cap_sign);
	denom = dot(normal, global->current_ray_dir);
	if (comp_floats(denom, 0) || denom > 0)
		return (intersec);
	t = dot(subtract(cap_center, global->current_ray_origin), normal) / denom;
	if (t < 0)
		return (intersec);
	hit_point = add(global->current_ray_origin,
			multiply(global->current_ray_dir, t));
	dist_from_center = magnitude(subtract(hit_point, cap_center));
	if (dist_from_center > cylinder->radius)
		return (intersec);
	intersec.dist = t;
	intersec.point = hit_point;
	intersec.obj_type = 2;
	intersec.obj_index = cy_id;
	return (intersec);
}

t_intersec	process_lateral_hit(t_global *global, int cy_id, float t)
{
	t_intersec	intersec;
	t_cylinder	*cylinder;
	t_vector	hit_point;
	float		hit_height;
	t_vector	center_at_height;
	t_vector	normal;

	intersec = init_intersec();
	if (t < 0)
		return (intersec);
	cylinder = &global->scene.cylinders[cy_id];
	hit_point = add(global->current_ray_origin,
			multiply(global->current_ray_dir, t));
	hit_height = dot(subtract(hit_point, cylinder->base),
			global->current_cyl_vars.axis);
	if (is_less_than(hit_height, 0) || is_greater_than(hit_height,
			cylinder->height))
		return (intersec);
	center_at_height = add(cylinder->base,
			multiply(global->current_cyl_vars.axis, hit_height));
	normal = normalize(subtract(hit_point, center_at_height));
	if (dot(normal, global->current_ray_dir) >= 0)
		return (intersec);
	intersec.dist = t;
	intersec.point = hit_point;
	intersec.obj_type = 2;
	intersec.obj_index = cy_id;
	return (intersec);
}

t_intersec	check_lateral_hits(t_global *global, int cy_id)
{
	t_intersec	hit1;
	t_intersec	hit2;

	hit1 = process_lateral_hit(global, cy_id, global->current_cyl_vars.t1);
	if (hit1.obj_type >= 0)
		return (hit1);
	hit2 = process_lateral_hit(global, cy_id, global->current_cyl_vars.t2);
	return (hit2);
}
