/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_cylinder_caps.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:10:10 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/22 21:10:53 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/**
 * Initializes cap variables for cylinder intersection calculations
 * Sets cap_center and normal vector based on which cap we're checking
 */
void	init_cap_vars(t_global *global, int cy_id, int cap_sign)
{
	t_cylinder	*cyl;
	t_cyl_lat	*vars;

	cyl = &global->scene.cyls[cy_id];
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
}

/**
 * Validates the intersection with cylinder cap and sets values if valid
 * Returns 1 if valid intersection, 0 otherwise
 */
int	validate_cap_hit(t_global *global, int cy_id, float t, t_intersec *isec)
{
	t_cylinder	*cyl;
	t_cyl_lat	*vars;

	cyl = &global->scene.cyls[cy_id];
	vars = &global->current_cyl_vars;
	vars->hit_point = add(global->c_ray.origin, multiply(global->c_ray.dir, t));
	vars->dist_from_center = mag(subtract(vars->hit_point, vars->cap_center));
	if (vars->dist_from_center > cyl->radius)
		return (0);
	isec->dist = t;
	isec->point = vars->hit_point;
	isec->obj_type = 2;
	isec->obj_index = cy_id;
	return (1);
}

/**
 * Calculates intersection between a ray and a cylinder cap
 * Returns intersection information if valid, otherwise default values
 */
t_intersec	cal_cap_isec(t_global *global, int cy_id, int cap_sign)
{
	t_intersec	isec;
	t_cyl_lat	*vars;
	float		denom;
	float		t;

	isec = init_isec();
	vars = &global->current_cyl_vars;
	init_cap_vars(global, cy_id, cap_sign);
	denom = dot(vars->normal, global->c_ray.dir);
	if (comp_floats(denom, 0) || denom > 0)
		return (isec);
	t = dot(subtract(vars->cap_center, global->c_ray.origin), vars->normal)
		/ denom;
	if (t < 0)
		return (isec);
	validate_cap_hit(global, cy_id, t, &isec);
	return (isec);
}
