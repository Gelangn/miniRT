#include "minirt.h"

float	cal_discriminant(t_vector oc, t_vector ray_dir, float radius)
{
	float	a;
	float	b;
	float	c;
	float	discriminant;

	a = dot(ray_dir, ray_dir);
	b = 2.0 * dot(oc, ray_dir);
	c = dot(oc, oc) - radius * radius;
	discriminant = b * b - 4 * a * c;
	return (discriminant);
}

t_intersec	col_sp(t_sphere *sphere, t_vector ray_origin, t_vector ray_dir)
{
	t_intersec	intersec;
	float		t1;
	float		t2;
	float		discriminant;
	t_vector	oc;

	intersec.dist = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	oc = subtract(ray_origin, sphere->center);
	discriminant = cal_discriminant(oc, ray_dir, sphere->radius);
	if (discriminant < 0)
		return (intersec);
	t1 = (-dot(ray_dir, oc) - sqrt(discriminant)) / dot(ray_dir, ray_dir);
	t2 = (-dot(ray_dir, oc) + sqrt(discriminant)) / dot(ray_dir, ray_dir);
	if (t1 > 0 && t2 > 0)
		intersec.dist = fmin(t1, t2);
	else if (t1 > 0)
		intersec.dist = t1;
	else if (t2 > 0)
		intersec.dist = t2;
	else
		return (intersec);
	intersec.point = add(ray_origin, multiply(ray_dir, intersec.dist));
	return (intersec);
}

t_intersec	col_pl(t_plane *plane, t_vector ray_origin, t_vector ray_dir)
{
	t_intersec	intersec;
	float		denom;
	float		t;
	t_vector	p0l0;

	intersec = init_intersec();
	denom = dot(plane->normal, ray_dir);
	if (fabs(denom) < EPSILON)
		return (intersec);
	p0l0 = subtract(plane->point, ray_origin);
	t = dot(p0l0, plane->normal) / denom;
	if (t < 0)
		return (intersec);
	intersec.dist = t;
	intersec.point = add(ray_origin, multiply(ray_dir, t));
	intersec.obj_type = 1;
	return (intersec);
}

t_intersec	col_cy(t_cylinder *cylinder, t_vector ray_origin, t_vector ray_dir)
{
	t_intersec	intersec;
	t_intersec	lateral_intersec;
	t_intersec	top_cap_intersec;
	t_intersec	bottom_cap_intersec;

	intersec = init_intersec();
	lateral_intersec = cal_lateral_intersec(cylinder, ray_origin, ray_dir);
	top_cap_intersec = cal_cap_intersec(cylinder, ray_origin, ray_dir, 1);
	bottom_cap_intersec = cal_cap_intersec(cylinder, ray_origin, ray_dir, -1);
	if (lateral_intersec.dist < intersec.dist)
		intersec = lateral_intersec;
	if (top_cap_intersec.dist < intersec.dist)
		intersec = top_cap_intersec;
	if (bottom_cap_intersec.dist < intersec.dist)
		intersec = bottom_cap_intersec;
	return (intersec);
}

t_vector	get_cap_center(t_cylinder *cylinder, t_vector axis, int cap_sign)
{
	if (cap_sign == 1)
		return (add(cylinder->base, multiply(axis, cylinder->height)));
	return (cylinder->base);
}

t_vector	get_cap_normal(t_vector axis, int cap_sign)
{
	if (cap_sign == 1)
		return (axis);
	return (multiply(axis, -1));
}

t_intersec	cal_cap_intersec(t_cylinder *cylinder, t_vector ray_origin,
		t_vector ray_dir, int cap_sign)
{
	t_intersec	intersec;
	t_vector	cap_center;
	t_vector	axis;
	float		t;
	t_vector	hit_point;
	float		dist_from_center;
	float		denom;
	t_vector	normal;

	intersec = init_intersec();
	axis = normalize(cylinder->orientation);
	cap_center = get_cap_center(cylinder, axis, cap_sign);
	normal = get_cap_normal(axis, cap_sign);
	denom = dot(normal, ray_dir);
	if (fabs(denom) < EPSILON || denom > 0)
		return (intersec);
	t = dot(subtract(cap_center, ray_origin), normal) / denom;
	if (t < 0)
		return (intersec);
	hit_point = add(ray_origin, multiply(ray_dir, t));
	dist_from_center = magnitude(subtract(hit_point, cap_center));
	if (dist_from_center > cylinder->radius)
		return (intersec);
	intersec.dist = t;
	intersec.point = hit_point;
	intersec.obj_type = 2;
	return (intersec);
}

t_intersec	cal_lateral_intersec(t_cylinder *cylinder, t_vector ray_origin,
		t_vector ray_dir)
{
	t_intersec	intersec;
	t_cyl_lat	vars;
	float		discriminant;

	intersec = init_intersec();
	init_lateral_intersec_vars(cylinder, ray_origin, ray_dir, &vars);
	discriminant = cal_lateral_discriminant(cylinder, vars);
	if (discriminant < 0)
		return (intersec);
	get_intersec_points(dot(vars.dir_perp, vars.dir_perp),
						2 * dot(vars.dir_perp, vars.oc_perp),
						discriminant,
						&vars);
	return (check_lateral_hits(cylinder, ray_origin, ray_dir, vars));
}


float	cal_lateral_discriminant(t_cylinder *cylinder, t_cyl_lat vars)
{
	float a, b, c, discriminant;
	a = dot(vars.dir_perp, vars.dir_perp);
	b = 2 * dot(vars.dir_perp, vars.oc_perp);
	c = dot(vars.oc_perp, vars.oc_perp) - (cylinder->radius * cylinder->radius);
	discriminant = b * b - 4 * a * c;
	return (a < EPSILON ? -1 : discriminant);
}

void	get_intersec_points(float a, float b, float discriminant,
		t_cyl_lat *vars)
{
	float	temp;

	vars->t1 = (-b - sqrt(discriminant)) / (2 * a);
	vars->t2 = (-b + sqrt(discriminant)) / (2 * a);
	if (vars->t1 > vars->t2)
	{
		temp = vars->t1;
		vars->t1 = vars->t2;
		vars->t2 = temp;
	}
}
