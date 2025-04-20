#include "minirt.h"

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
	// Calcular intersecciones con las diferentes partes del cilindro
	lateral_intersec = cal_lateral_intersec(global, cy_id);
	top_cap_intersec = cal_cap_intersec(global, cy_id, 1);
	bottom_cap_intersec = cal_cap_intersec(global, cy_id, -1);
	// Tomar la intersección más cercana
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

// Función de intersección de tapas simplificada
t_intersec	cal_cap_intersec(t_global *global, int cy_id, int cap_sign)
{
	t_intersec	intersec;
	t_cylinder	*cylinder;
	t_vector	cap_center;
	t_vector	normal;
	float		denom;
	float		t;
	t_vector	hit_point;
	float		dist_from_center;

	intersec = init_intersec();
	cylinder = &global->scene.cylinders[cy_id];
	cap_center = get_cap_center(cylinder, cap_sign);
	normal = get_cap_normal(cylinder, cap_sign);
	denom = dot(normal, global->current_ray_dir);
	if (comp_floats(denom, 0) || denom > 0)
		return (intersec);
	t = dot(subtract(cap_center, global->current_ray_origin), normal) / denom;
	if (t < 0)
		return (intersec);
	hit_point = add(global->current_ray_origin,
			multiply(global->current_ray_dir, t));
	dist_from_center = magnitude(subtract(hit_point, cap_center));
	if (dist_from_center > cylinder->radius)
		return (intersec);
	intersec.dist = t;
	intersec.point = hit_point;
	intersec.obj_type = 2;
	intersec.obj_index = cy_id;
	return (intersec);
}

t_intersec	cal_lateral_intersec(t_global *global, int cy_id)
{
	t_intersec	intersec;
	float		discriminant;

	intersec = init_intersec();
	// Inicializar variables en la estructura global
	init_lateral_intersec_vars(global, cy_id);
	discriminant = cal_lateral_discriminant(global, cy_id);
	if (discriminant < 0)
		return (intersec);
	get_intersec_points(global, dot(global->current_cyl_vars.dir_perp,
			global->current_cyl_vars.dir_perp), 2
		* dot(global->current_cyl_vars.dir_perp,
			global->current_cyl_vars.oc_perp), discriminant);
	intersec = check_lateral_hits(global, cy_id);
	return (intersec);
}

// Versión simplificada - usando current_cyl_vars en global
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
