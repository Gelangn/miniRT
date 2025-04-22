/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colisions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:44:21 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/22 20:48:04 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vector	get_cap_center(t_cylinder *cyl, int cap_sign)
{
	t_vector	axis;

	axis = norm(cyl->orientation);
	if (cap_sign == 1)
		return (add(cyl->base, multiply(axis, cyl->height)));
	return (cyl->base);
}

t_vector	get_cap_normal(t_cylinder *cyl, int cap_sign)
{
	t_vector	axis;

	axis = norm(cyl->orientation);
	if (cap_sign == 1)
		return (axis);
	return (multiply(axis, -1));
}

/* float	cal_lat_discriminant(t_global *global, int cy_id)
{
	t_cylinder	*cyl;
	t_cyl_lat	*vars;

	cyl = &global->scene.cyls[cy_id];
	vars = &global->current_cyl_vars;
	vars->a = dot(vars->dir_perp, vars->dir_perp);
	vars->b = 2 * dot(vars->dir_perp, vars->oc_perp);
	vars->c = dot(vars->oc_perp, vars->oc_perp) - cyl->radius * cyl->radius;
	vars->discr = vars->b * vars->b - 4 * vars->a * vars->c;
	return (vars->discr);
} */

void	get_isec_points(t_global *global, float a, float b, float discr)
{
	float		sqrt_disc;
	t_cyl_lat	*vars;

	vars = &global->current_cyl_vars;
	sqrt_disc = sqrt(discr);
	vars->t1 = (-b - sqrt_disc) / (2 * a);
	vars->t2 = (-b + sqrt_disc) / (2 * a);
}
