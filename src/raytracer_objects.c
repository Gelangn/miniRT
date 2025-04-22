/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_objects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:33:03 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/22 20:18:15 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Calcula la intersección entre un rayo y una esfera.
 * Combina las antiguas funciones cal_discriminant() y col_sp().
 * discriminante: at² + bt + c = 0
 */
t_intersec	col_sp(t_global *global, int sp_id)
{
	t_intersec	isec;
	t_vector	oc;
	float		discr;
	float		abc[3];
	float		t[2];

	isec = init_intersec();
	oc = subtract(global->c_ray.origin, global->scene.spheres[sp_id].center);
	abc[0] = dot(global->c_ray.dir, global->c_ray.dir);
	abc[1] = 2.0 * dot(oc, global->c_ray.dir);
	abc[2] = dot(oc, oc) - pow(global->scene.spheres[sp_id].radius, 2);
	discr = abc[1] * abc[1] - 4 * abc[0] * abc[2];
	if (discr >= 0)
	{
		t[0] = (-abc[1] - sqrt(discr)) / (2 * abc[0]);
		t[1] = (-abc[1] + sqrt(discr)) / (2 * abc[0]);
		if (t[0] > 0 && t[0] < t[1])
			isec.dist = t[0];
		else if (t[1] > 0)
			isec.dist = t[1];
		if (isec.dist < INFINITY)
			isec.point = add(global->c_ray.origin,
					multiply(global->c_ray.dir, isec.dist));
	}
	return (isec);
}

t_intersec	col_pl(t_global *global, int pl_id)
{
	t_intersec	isec;
	float		denom;
	float		t;
	t_vector	p0l0;
	t_plane		*plane;

	plane = &global->scene.planes[pl_id];
	isec = init_intersec();
	denom = dot(plane->normal, global->c_ray.dir);
	if (comp_floats(denom, 0) || denom > 0)
		return (isec);
	p0l0 = subtract(plane->point, global->c_ray.origin);
	t = dot(p0l0, plane->normal) / denom;
	if (t < 0)
		return (isec);
	isec.dist = t;
	isec.point = add(global->c_ray.origin, multiply(global->c_ray.dir, t));
	isec.obj_type = 1;
	return (isec);
}

t_intersec	col_cy(t_global *global, int cy_id)
{
	t_intersec	isec;
	t_intersec	lateral_intersec;
	t_intersec	top_cap_intersec;
	t_intersec	bottom_cap_intersec;

	isec = init_intersec();
	lateral_intersec = cal_lateral_intersec(global, cy_id);
	top_cap_intersec = cal_cap_intersec(global, cy_id, 1);
	bottom_cap_intersec = cal_cap_intersec(global, cy_id, -1);
	if (lateral_intersec.dist < isec.dist)
		isec = lateral_intersec;
	if (top_cap_intersec.dist < isec.dist)
		isec = top_cap_intersec;
	if (bottom_cap_intersec.dist < isec.dist)
		isec = bottom_cap_intersec;
	if (isec.dist < INFINITY)
	{
		isec.obj_type = 2;
		isec.obj_index = cy_id;
	}
	return (isec);
}
