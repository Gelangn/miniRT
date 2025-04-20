/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colisions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:44:21 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 22:47:48 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	get_cap_center(t_cylinder *cylinder, int cap_sign)
{
	t_vector	axis;

	axis = norm(cylinder->orientation);
	if (cap_sign == 1)
		return (add(cylinder->base, multiply(axis, cylinder->height)));
	return (cylinder->base);
}

t_vector	get_cap_normal(t_cylinder *cylinder, int cap_sign)
{
	t_vector	axis;

	axis = norm(cylinder->orientation);
	if (cap_sign == 1)
		return (axis);
	return (multiply(axis, -1));
}

float	cal_lat_discriminant(t_global *global, int cy_id)
{
	t_cylinder	*cylinder;
	t_cyl_lat	*vars;

	cylinder = &global->scene.cylinders[cy_id];
	vars = &global->current_cyl_vars;
	vars->a = dot(vars->dir_perp, vars->dir_perp);
	vars->b = 2 * dot(vars->dir_perp, vars->oc_perp);
	vars->c = dot(vars->oc_perp, vars->oc_perp) - cylinder->radius
		* cylinder->radius;
	vars->discr = vars->b * vars->b - 4 * vars->a * vars->c;
	return (vars->discr);
}

void	get_intersec_points(t_global *global, float a, float b, float discr)
{
	float		sqrt_disc;
	t_cyl_lat	*vars;

	vars = &global->current_cyl_vars;
	sqrt_disc = sqrt(discr);
	vars->t1 = (-b - sqrt_disc) / (2 * a);
	vars->t2 = (-b + sqrt_disc) / (2 * a);
}
