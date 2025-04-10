// incluir cabecera 42

#include "../inc/minirt.h"

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

t_intersec	init_intersec(void)
{
	t_intersec	intersec;

	intersec.dist = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	intersec.obj_index = -1;
	intersec.obj_type = -1;
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

void	init_lateral_intersec_vars(t_cylinder *cylinder, t_vector ray_origin,
		t_vector ray_dir, t_cyl_lat *vars)
{
	vars->axis = normalize(cylinder->orientation);
	vars->oc = subtract(ray_origin, cylinder->base);
	vars->dir_dot_axis = dot(ray_dir, vars->axis);
	vars->oc_dot_axis = dot(vars->oc, vars->axis);
	vars->dir_perp = subtract(ray_dir, multiply(vars->axis,
				vars->dir_dot_axis));
	vars->oc_perp = subtract(vars->oc, multiply(vars->axis, vars->oc_dot_axis));
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

t_intersec	process_lateral_hit(t_cylinder *cylinder, t_vector ray_origin,
		t_vector ray_dir, t_cyl_lat vars, float t)
{
	t_intersec	intersec;
	t_vector	hit_point;
	float		hit_height;
	t_vector	hit_to_axis;
	t_vector	center_at_height;
	t_vector	normal;

	intersec = init_intersec();
	if (t < 0)
		return (intersec);
	hit_point = add(ray_origin, multiply(ray_dir, t));
	hit_height = dot(subtract(hit_point, cylinder->base), vars.axis);
	if (hit_height < -EPSILON || hit_height > cylinder->height + EPSILON)
		return (intersec);
	hit_to_axis = multiply(vars.axis, hit_height);
	center_at_height = add(cylinder->base, hit_to_axis);
	normal = normalize(subtract(hit_point, center_at_height));
	if (dot(normal, ray_dir) >= 0)
		return (intersec);
	intersec.dist = t;
	intersec.point = hit_point;
	intersec.obj_type = 2;
	return (intersec);
}

t_intersec	check_lateral_hits(t_cylinder *cylinder, t_vector ray_origin,
		t_vector ray_dir, t_cyl_lat vars)
{
	t_intersec	hit1;
	t_intersec	hit2;
	float		t;

	t = vars.t1;
	hit1 = process_lateral_hit(cylinder, ray_origin, ray_dir, vars, t);
	if (hit1.obj_type >= 0)
		return (hit1);
	t = vars.t2;
	hit2 = process_lateral_hit(cylinder, ray_origin, ray_dir, vars, t);
	return (hit2);
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

t_vector	get_ray_direction(t_camera cam, int pixel_x, int pixel_y)
{
	float		u;
	float		v;
	float		scrn_w;
	float		scrn_h;
	float		aspect_ratio;
	t_vector	forward;
	t_vector	right;
	t_vector	up;
	t_vector	ray_dir;

	aspect_ratio = (float)WIN_W / (float)WIN_H;
	scrn_w = 2.0 * DSCR * tan((cam.fov * PI / 180.0) / 2.0);
	scrn_h = scrn_w / aspect_ratio;
	forward = normalize(cam.orientation);
	right = normalize(cross((t_vector){0, 1, 0}, forward));
	up = cross(forward, right);
	u = (2 * ((pixel_x + 0.5) / WIN_W) - 1) * scrn_w / 2;
	v = (1 - 2 * ((pixel_y + 0.5) / WIN_H)) * scrn_h / 2;
	ray_dir = normalize(add(add(multiply(right, u), multiply(up, v)), forward));
	return (ray_dir);
}

void	check_sphere_intersecs(t_global *global, t_vector ray_origin,
		t_vector ray_dir, t_intersec *closest_intersec)
{
	t_sphere	*sphere;
	t_intersec	temp_intersec;
	int			i;

	sphere = global->scene.spheres;
	i = -1;
	while (++i < global->scene.num_sp)
	{
		temp_intersec = col_sp(&sphere[i], ray_origin, ray_dir);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = 0;
		}
	}
}

void	check_plane_intersecs(t_global *global, t_vector ray_origin,
		t_vector ray_dir, t_intersec *closest_intersec)
{
	t_plane		*plane;
	t_intersec	temp_intersec;
	int			i;

	plane = global->scene.planes;
	i = -1;
	while (++i < global->scene.num_pl)
	{
		temp_intersec = col_pl(&plane[i], ray_origin, ray_dir);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = 1;
		}
	}
}

void	check_cylinder_intersecs(t_global *global, t_vector ray_origin,
		t_vector ray_dir, t_intersec *closest_intersec)
{
	t_cylinder	*cylinder;
	t_intersec	temp_intersec;
	int			i;

	cylinder = global->scene.cylinders;
	i = -1;
	while (++i < global->scene.num_cy)
	{
		temp_intersec = col_cy(&cylinder[i], ray_origin, ray_dir);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = 2;
		}
	}
}

t_intersec	find_closest_intersec(t_global *global, t_vector ray_origin,
		t_vector ray_dir)
{
	t_intersec	closest_intersec;

	closest_intersec = init_intersec();
	check_sphere_intersecs(global, ray_origin, ray_dir, &closest_intersec);
	check_plane_intersecs(global, ray_origin, ray_dir, &closest_intersec);
	check_cylinder_intersecs(global, ray_origin, ray_dir, &closest_intersec);
	return (closest_intersec);
}

t_intersec	cal_ray(t_global *global, int pixel_x, int pixel_y)
{
	t_vector	ray_dir;
	t_camera	cam;
	t_intersec	intersec;

	cam = global->scene.cam;
	ray_dir = get_ray_direction(cam, pixel_x, pixel_y);
	intersec = find_closest_intersec(global, cam.pos, ray_dir);
	return (intersec);
}

void	render_pixel(t_global *global, t_intersec intersec, t_img *img, int x,
		int y)
{
	int			color;
	t_vector	normal;
	float		dot_normal_axis;

	if (intersec.obj_type == 0 && intersec.obj_index >= 0)
		color = rgb_to_int(global->scene.spheres[intersec.obj_index].color);
	else if (intersec.obj_type == 1 && intersec.obj_index >= 0)
		color = rgb_to_int(global->scene.planes[intersec.obj_index].color);
	else if (intersec.obj_type == 2 && intersec.obj_index >= 0)
	{
		normal = get_surface_normal(global, intersec);
		dot_normal_axis = fabs(dot(normal,
									global->scene.cylinders[intersec.obj_index].orientation));
		if (dot_normal_axis > 0.99)
			color = RED;
		else
			color = CYAN;
	}
	else
		color = DARK_GREY;
	pixel_put(img, x, y, color);
}

void	cal_all_intersecs(t_global *global, t_intersec *intersecs)
{
	int	x;
	int	y;
	int	i;

	i = 0;
	x = -1;
	while (++x < WIN_W)
	{
		y = -1;
		while (++y < WIN_H)
		{
			intersecs[i++] = cal_ray(global, x, y);
		}
	}
	printf("Total calculate rays: %d\n", i);
}

void	render_all_pixels(t_global *global, t_intersec *intersecs)
{
	int	x;
	int	y;
	int	i;

	i = 0;
	x = -1;
	while (++x < WIN_W)
	{
		y = -1;
		while (++y < WIN_H)
		{
			render_pixel(global, intersecs[i++], &global->img, x, y);
		}
	}
}

void	render(t_global *global)
{
	t_intersec	*intersecs;

	intersecs = malloc(WIN_W * WIN_H * sizeof(t_intersec));
	if (!intersecs)
		return ;
	cal_all_intersecs(global, intersecs);
	render_all_pixels(global, intersecs);
	free(intersecs);
}

t_vector	get_sphere_normal(t_global *global, t_intersec intersec)
{
	t_sphere	sphere;

	sphere = global->scene.spheres[intersec.obj_index];
	return (normalize(subtract(intersec.point, sphere.center)));
}

t_vector	get_plane_normal(t_global *global, t_intersec intersec)
{
	t_plane	plane;

	plane = global->scene.planes[intersec.obj_index];
	return (normalize(plane.normal));
}

t_vector	get_cylinder_normal(t_global *global, t_intersec intersec)
{
	t_cylinder	cylinder;
	t_vector	axis;
	float		hit_height;
	t_vector	center_at_height;

	cylinder = global->scene.cylinders[intersec.obj_index];
	axis = normalize(cylinder.orientation);
	hit_height = dot(subtract(intersec.point, cylinder.base), axis);
	if (hit_height < EPSILON)
		return (multiply(axis, -1));
	else if (fabs(hit_height - cylinder.height) < EPSILON)
		return (axis);
	else
	{
		center_at_height = add(cylinder.base, multiply(axis, hit_height));
		return (normalize(subtract(intersec.point, center_at_height)));
	}
}

t_vector	get_surface_normal(t_global *global, t_intersec intersec)
{
	t_vector	normal;

	normal = (t_vector){0, 0, 0};
	if (intersec.obj_type == 0)
		normal = get_sphere_normal(global, intersec);
	else if (intersec.obj_type == 1)
		normal = get_plane_normal(global, intersec);
	else if (intersec.obj_type == 2)
		normal = get_cylinder_normal(global, intersec);
	return (normal);
}
