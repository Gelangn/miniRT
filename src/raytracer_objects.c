/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_objects.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:33:03 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 22:04:29 by anavas-g         ###   ########.fr       */
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

	oc = subtract(global->c_ray.origin, center);
	ray_dir = global->c_ray.dir;
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
	t_intersec	isec;
	t_vector	oc;
	float		discr;
	float		t1;
	float		t2;
	t_sphere	*sphere;

	sphere = &global->scene.spheres[sp_id];
	isec = init_intersec();
	oc = subtract(global->c_ray.origin, sphere->center);
	discr = cal_discriminant(global, sphere->center, sphere->radius);
	if (discr < 0)
		return (isec);
	t1 = (-dot(global->c_ray.dir, oc) - sqrt(discr)) / dot(global->c_ray.dir,
			global->c_ray.dir);
	t2 = (-dot(global->c_ray.dir, oc) + sqrt(discr)) / dot(global->c_ray.dir,
			global->c_ray.dir);
	if (t1 > 0 && t1 < t2)
		isec.dist = t1;
	else if (t2 > 0)
		isec.dist = t2;
	else
		return (isec);
	isec.point = add(global->c_ray.origin, multiply(global->c_ray.dir, isec.dist));
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
	if (comp_floats(denom, 0))
		return (isec);
	p0l0 = subtract(plane->point, global->c_ray.origin);
	t = dot(p0l0, plane->normal) / denom;
	if (t < 0)
		return (isec);
	isec.dist = t;
	isec.point = add(global->c_ray.origin,
						multiply(global->c_ray.dir, t));
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
