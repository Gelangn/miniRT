// incluir cabecera 42

#include "../inc/minirt.h"

t_intersec	process_lateral_hit(t_cylinder *cylinder, t_global *global,
		t_cyl_lat vars, float t)
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
	hit_point = add(global->current_ray_origin,
			multiply(global->current_ray_dir, t));
	hit_height = dot(subtract(hit_point, cylinder->base), vars.axis);
	if (is_less_than(hit_height, 0) || is_greater_than(hit_height,
			cylinder->height))
		return (intersec);
	hit_to_axis = multiply(vars.axis, hit_height);
	center_at_height = add(cylinder->base, hit_to_axis);
	normal = normalize(subtract(hit_point, center_at_height));
	if (dot(normal, global->current_ray_dir) >= 0)
		return (intersec);
	intersec.dist = t;
	intersec.point = hit_point;
	intersec.obj_type = 2;
	return (intersec);
}

t_intersec	check_lateral_hits(t_cylinder *cylinder, t_global *global,
		t_cyl_lat vars)
{
	t_intersec	hit1;
	t_intersec	hit2;
	float		t;

	t = vars.t1;
	hit1 = process_lateral_hit(cylinder, global, vars, t);
	if (hit1.obj_type >= 0)
		return (hit1);
	t = vars.t2;
	hit2 = process_lateral_hit(cylinder, global, vars, t);
	return (hit2);
}

void	check_sp_intersecs(t_global *global, t_intersec *closest_intersec)
{
	t_sphere	*sphere;
	t_intersec	temp_intersec;
	int			i;

	sphere = global->scene.spheres;
	i = -1;
	while (++i < global->scene.num_sp)
	{
		temp_intersec = col_sp(&sphere[i], global);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = 0;
		}
	}
}

void	check_pl_intersecs(t_global *global, t_intersec *closest_intersec)
{
	t_plane		*plane;
	t_intersec	temp_intersec;
	int			i;

	plane = global->scene.planes;
	i = -1;
	while (++i < global->scene.num_pl)
	{
		temp_intersec = col_pl(&plane[i], global);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = 1;
		}
	}
}

void	check_cy_intersecs(t_global *global, t_intersec *closest_intersec)
{
	t_cylinder	*cylinder;
	t_intersec	temp_intersec;
	int			i;

	cylinder = global->scene.cylinders;
	i = -1;
	while (++i < global->scene.num_cy)
	{
		temp_intersec = col_cy(&cylinder[i], global);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = 2;
		}
	}
}

t_intersec	find_closest_intersec(t_global *global)
{
	t_intersec	closest_intersec;

	closest_intersec = init_intersec();
	if (global->scene.num_sp > 0)
		check_sp_intersecs(global, &closest_intersec);
	if (global->scene.num_pl > 0)
		check_pl_intersecs(global, &closest_intersec);
	if (global->scene.num_cy > 0)
		check_cy_intersecs(global, &closest_intersec);
	return (closest_intersec);
}

void	trace_all_rays(t_global *global, t_intersec *intersecs)
{
	int	x;
	int	y;
	int	i;

	i = 0;
	x = -1;
	// Solo calcular para el tamaño de la imagen
	while (++x < (WIN_W - MARGIN))
	{
		y = -1;
		while (++y < (WIN_H - MARGIN))
		{
			intersecs[i++] = cal_ray(global, x, y);
		}
	}
	printf("Total calculate rays: %d\n", i);
}

t_intersec	cal_ray(t_global *global, int pixel_x, int pixel_y)
{
	t_camera	cam;
	t_intersec	intersec;

	cam = global->scene.cam;
	global->current_ray_origin = cam.pos;
	global->current_ray_dir = get_ray_direction(cam, pixel_x, pixel_y);
	intersec = find_closest_intersec(global);
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
	// Cambiamos el vector de referencia para Y (ahora Y apunta hacia abajo)
	right = normalize(cross((t_vector){0, -1, 0}, forward));
	up = cross(forward, right);
	u = (2 * ((pixel_x + 0.5) / WIN_W) - 1) * scrn_w / 2;
	// Invertimos el cálculo de v para que crezca hacia abajo
	v = (2 * ((pixel_y + 0.5) / WIN_H) - 1) * scrn_h / 2;
	ray_dir = normalize(add(add(multiply(right, u), multiply(up, v)), forward));
	return (ray_dir);
}

t_vector	get_sp_normal(t_global *global, t_intersec intersec)
{
	t_sphere	sphere;

	sphere = global->scene.spheres[intersec.obj_index];
	return (normalize(subtract(intersec.point, sphere.center)));
}

t_vector	get_pl_normal(t_global *global, t_intersec intersec)
{
	t_plane	plane;

	if (intersec.obj_index < 0 || intersec.obj_index >= global->scene.num_pl)
		return ((t_vector){0, 1, 0}); // Valor predeterminado seguro
	plane = global->scene.planes[intersec.obj_index];
	return (normalize(plane.normal));
}

t_vector	get_cy_normal(t_global *global, t_intersec intersec)
{
	t_cylinder	cylinder;
	t_vector	axis;
	float		hit_height;
	t_vector	center_at_height;

	cylinder = global->scene.cylinders[intersec.obj_index];
	axis = normalize(cylinder.orientation);
	hit_height = dot(subtract(intersec.point, cylinder.base), axis);
	if (comp_floats(hit_height, 0))
		return (multiply(axis, -1));
	else if (comp_floats(hit_height, cylinder.height))
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
	if (intersec.obj_type == 0 && intersec.obj_index >= 0
		&& intersec.obj_index < global->scene.num_sp)
		normal = get_sp_normal(global, intersec);
	else if (intersec.obj_type == 1 && intersec.obj_index >= 0
		&& intersec.obj_index < global->scene.num_pl)
		normal = get_pl_normal(global, intersec);
	else if (intersec.obj_type == 2 && intersec.obj_index >= 0
		&& intersec.obj_index < global->scene.num_cy)
		normal = get_cy_normal(global, intersec);
	return (normal);
}
