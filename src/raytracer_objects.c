/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_objects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:33:03 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 21:10:59 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

float	cal_discriminant(t_global *global, t_vector center, float radius)
{
	t_vector	oc;
	t_vector	ray_dir;
	float		a;
	float		b;
	float		c;

	oc = subtract(global->current_ray_origin, center);
	ray_dir = global->current_ray_dir;
	a = dot(ray_dir, ray_dir);
	b = 2.0 * dot(oc, ray_dir);
	c = dot(oc, oc) - radius * radius;
	return (b * b - 4 * a * c);
}

void	check_obj_intersecs(t_global *global, t_intersec *closest, int obj_type)
{
	int			i;
	int			max_objs;
	t_intersec	temp;

	if (obj_type == 0)
		max_objs = global->scene.num_sp;
	else if (obj_type == 1)
		max_objs = global->scene.num_pl;
	else
		max_objs = global->scene.num_cy;
	i = -1;
	while (++i < max_objs)
	{
		if (obj_type == 0)
			temp = col_sp(global, i);
		else if (obj_type == 1)
			temp = col_pl(global, i);
		else
			temp = col_cy(global, i);
		if (temp.dist < closest->dist)
		{
			*closest = temp;
			closest->obj_index = i;
			closest->obj_type = obj_type;
		}
	}
}

t_intersec	col_sp(t_global *global, int sp_id)
{
	t_intersec	intersec;
	t_vector	oc;
	float		discriminant;
	float		t1;
	float		t2;
	t_sphere	*sphere;

	sphere = &global->scene.spheres[sp_id];
	intersec = init_intersec();
	oc = subtract(global->current_ray_origin, sphere->center);
	discriminant = cal_discriminant(global, sphere->center, sphere->radius);
	if (discriminant < 0)
		return (intersec);
	t1 = (-dot(global->current_ray_dir, oc) - sqrt(discriminant))
		/ dot(global->current_ray_dir, global->current_ray_dir);
	t2 = (-dot(global->current_ray_dir, oc) + sqrt(discriminant))
		/ dot(global->current_ray_dir, global->current_ray_dir);
	if (t1 > 0 && t1 < t2)
		intersec.dist = t1;
	else if (t2 > 0)
		intersec.dist = t2;
	else
		return (intersec);
	intersec.point = add(global->current_ray_origin,
			multiply(global->current_ray_dir, intersec.dist));
	return (intersec);
}

t_intersec	col_pl(t_global *global, int pl_id)
{
	t_intersec	intersec;
	float		denom;
	float		t;
	t_vector	p0l0;
	t_plane		*plane;

	plane = &global->scene.planes[pl_id];
	intersec = init_intersec();
	denom = dot(plane->normal, global->current_ray_dir);
	if (comp_floats(denom, 0))
		return (intersec);
	p0l0 = subtract(plane->point, global->current_ray_origin);
	t = dot(p0l0, plane->normal) / denom;
	if (t < 0)
		return (intersec);
	intersec.dist = t;
	intersec.point = add(global->current_ray_origin,
			multiply(global->current_ray_dir, t));
	intersec.obj_type = 1;
	return (intersec);
}

t_intersec	col_cy(t_global *global, int cy_id)
{
	t_intersec	intersec;
	t_intersec	lateral_intersec;
	t_intersec	top_cap_intersec;
	t_intersec	bottom_cap_intersec;

	intersec = init_intersec();
	lateral_intersec = cal_lateral_intersec(global, cy_id);
	top_cap_intersec = cal_cap_intersec(global, cy_id, 1);
	bottom_cap_intersec = cal_cap_intersec(global, cy_id, -1);
	if (lateral_intersec.dist < intersec.dist)
		intersec = lateral_intersec;
	if (top_cap_intersec.dist < intersec.dist)
		intersec = top_cap_intersec;
	if (bottom_cap_intersec.dist < intersec.dist)
		intersec = bottom_cap_intersec;
	if (intersec.dist < INFINITY)
	{
		intersec.obj_type = 2;
		intersec.obj_index = cy_id;
	}
	return (intersec);
}
