/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_cyllinder.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:32:59 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 22:19:11 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_intersec	cal_lateral_intersec(t_global *global, int cy_id)
{
	t_intersec	isec;
	float		discr;

	isec = init_intersec();
	init_lateral_intersec_vars(global, cy_id);
	discr = cal_lat_discriminant(global, cy_id);
	if (discr < 0)
		return (isec);
	get_intersec_points(global, dot(global->current_cyl_vars.dir_perp,
				global->current_cyl_vars.dir_perp), 2
			* dot(global->current_cyl_vars.dir_perp,
				global->current_cyl_vars.oc_perp), discr);
	isec = check_lateral_hits(global, cy_id);
	return (isec);
}

t_intersec	cal_cap_intersec(t_global *global, int cy_id, int cap_sign)
{
	t_intersec	isec;
	t_cylinder	*cyl;
	t_cyl_lat	*vars;
	float		denom;
	float		t;

	isec = init_intersec();
	cyl = &global->scene.cylinders[cy_id];
	vars = &global->current_cyl_vars;
	if (cap_sign == 1)
	{
		vars->cap_center = add(cyl->base, multiply(vars->axis, cyl->height));
		vars->normal = vars->axis;
	}
	else
	{
		vars->cap_center = cyl->base;
		vars->normal = multiply(vars->axis, -1);
	}
	denom = dot(vars->normal, global->c_ray.dir);
	if (comp_floats(denom, 0) || denom > 0)
		return (isec);
	t = dot(subtract(vars->cap_center, global->c_ray.origin),
			vars->normal) / denom;
	if (t < 0)
		return (isec);
	vars->hit_point = add(global->c_ray.origin, multiply(global->c_ray.dir, t));
	vars->dist_from_center = mag(subtract(vars->hit_point, vars->cap_center));
	if (vars->dist_from_center > cyl->radius)
		return (isec);
	isec.dist = t;
	isec.point = vars->hit_point;
	isec.obj_type = 2;
	isec.obj_index = cy_id;
	return (isec);
}

t_intersec	process_lateral_hit(t_global *global, int cy_id, float t)
{
	t_intersec	isec;
	t_cylinder	*cyl;
	t_cyl_lat	*vars;

	isec = init_intersec();
	if (t < 0)
		return (isec);
	cyl = &global->scene.cylinders[cy_id];
	vars = &global->current_cyl_vars;
	vars->hit_point = add(global->c_ray.origin, multiply(global->c_ray.dir, t));
	vars->hit_h = dot(subtract(vars->hit_point, cyl->base), vars->axis);
	if (is_less_than(vars->hit_h, 0) || is_greater_than(vars->hit_h,
			cyl->height))
		return (isec);
	vars->ctr_h = add(cyl->base, multiply(vars->axis, vars->hit_h));
	vars->normal = norm(subtract(vars->hit_point, vars->ctr_h));
	if (dot(vars->normal, global->c_ray.dir) >= 0)
		return (isec);
	isec.dist = t;
	isec.point = vars->hit_point;
	isec.obj_type = 2;
	isec.obj_index = cy_id;
	return (isec);
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
