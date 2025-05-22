/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_cylinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:32:59 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/22 20:57:34 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_intersec	cal_lateral_isec(t_global *global, int cy_id)
{
	t_intersec	isec;
	t_cyl_lat	*vars;
	float		discr;
	float		a;
	float		sqrt_discr;

	isec = init_isec();
	init_lateral_isec_vars(global, cy_id);
	vars = &global->current_cyl_vars;
	a = dot(vars->dir_perp, vars->dir_perp);
	vars->b = 2.0 * dot(vars->dir_perp, vars->oc_perp);
	vars->c = dot(vars->oc_perp, vars->oc_perp)
		- pow(global->scene.cyls[cy_id].radius, 2);
	discr = vars->b * vars->b - 4 * a * vars->c;
	if (discr >= 0)
	{
		sqrt_discr = sqrt(discr);
		vars->t1 = (-vars->b - sqrt_discr) / (2 * a);
		vars->t2 = (-vars->b + sqrt_discr) / (2 * a);
		isec = check_lateral_hits(global, cy_id);
	}
	return (isec);
}

t_intersec	process_lateral_hit(t_global *global, int cy_id, float t)
{
	t_intersec	isec;
	t_cylinder	*cyl;
	t_cyl_lat	*vars;

	isec = init_isec();
	if (t < 0)
		return (isec);
	cyl = &global->scene.cyls[cy_id];
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
