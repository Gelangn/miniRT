/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colisions.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:44:21 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 20:44:25 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// Funciones auxiliares simplificadas
t_vector	get_cap_center(t_cylinder *cylinder, int cap_sign)
{
	t_vector	axis;

	axis = normalize(cylinder->orientation);
	if (cap_sign == 1)
		return (add(cylinder->base, multiply(axis, cylinder->height)));
	return (cylinder->base);
}

t_vector	get_cap_normal(t_cylinder *cylinder, int cap_sign)
{
	t_vector	axis;

	axis = normalize(cylinder->orientation);
	if (cap_sign == 1)
		return (axis);
	return (multiply(axis, -1));
}

float	cal_lateral_discriminant(t_global *global, int cy_id)
{
	t_cylinder	*cylinder;
	float		a;
	float		b;
	float		c;

	cylinder = &global->scene.cylinders[cy_id];
	a = dot(global->current_cyl_vars.dir_perp,
			global->current_cyl_vars.dir_perp);
	b = 2 * dot(global->current_cyl_vars.dir_perp,
			global->current_cyl_vars.oc_perp);
	c = dot(global->current_cyl_vars.oc_perp, global->current_cyl_vars.oc_perp)
		- cylinder->radius * cylinder->radius;
	return (b * b - 4 * a * c);
}

// Versión simplificada - usando current_cyl_vars en global
void	get_intersec_points(t_global *global, float a, float b,
		float discriminant)
{
	float	sqrt_disc;

	sqrt_disc = sqrt(discriminant);
	global->current_cyl_vars.t1 = (-b - sqrt_disc) / (2 * a);
	global->current_cyl_vars.t2 = (-b + sqrt_disc) / (2 * a);
}
