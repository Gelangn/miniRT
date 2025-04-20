// Include 42 header

#include "../inc/minirt.h"

t_intersec	process_lateral_hit(t_global *global, int cy_id, float t)
{
	t_intersec	intersec;
	t_cylinder	*cylinder;
	t_vector	hit_point;
	float		hit_height;
	t_vector	center_at_height;
	t_vector	normal;

	intersec = init_intersec();
	cylinder = &global->scene.cylinders[cy_id];
	if (t < 0)
		return (intersec);
	hit_point = add(global->current_ray_origin,
			multiply(global->current_ray_dir, t));
	hit_height = dot(subtract(hit_point, cylinder->base),
			global->current_cyl_vars.axis);
	if (is_less_than(hit_height, 0) || is_greater_than(hit_height,
			cylinder->height))
		return (intersec);
	center_at_height = add(cylinder->base,
			multiply(global->current_cyl_vars.axis, hit_height));
	normal = normalize(subtract(hit_point, center_at_height));
	if (dot(normal, global->current_ray_dir) >= 0)
		return (intersec);
	intersec.dist = t;
	intersec.point = hit_point;
	intersec.obj_type = 2;
	intersec.obj_index = cy_id;
	return (intersec);
}

t_intersec	check_lateral_hits(t_global *global, int cy_id)
{
	t_intersec	hit1;
	t_intersec	hit2;

	hit1 = process_lateral_hit(global, cy_id, global->current_cyl_vars.t1);
	if (hit1.obj_type >= 0)
		return (hit1);
	hit2 = process_lateral_hit(global, cy_id, global->current_cyl_vars.t2);
	return (hit2);
}

void	check_sp_intersecs(t_global *global, t_intersec *closest_intersec)
{
	int			i;
	t_intersec	temp_intersec;

	i = -1;
	while (++i < global->scene.num_sp)
	{
		temp_intersec = col_sp(global, i);
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
	int			i;
	t_intersec	temp_intersec;

	i = -1;
	while (++i < global->scene.num_pl)
	{
		temp_intersec = col_pl(global, i);
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
	int			i;
	t_intersec	temp_intersec;

	i = -1;
	while (++i < global->scene.num_cy)
	{
		temp_intersec = col_cy(global, i);
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
		check_obj_intersecs(global, &closest_intersec, 0);
	if (global->scene.num_pl > 0)
		check_obj_intersecs(global, &closest_intersec, 1);
	if (global->scene.num_cy > 0)
		check_obj_intersecs(global, &closest_intersec, 2);
	return (closest_intersec);
}

/* Core of the raytracing, calls cal_ray and in turn get_ray_direction */
void	trace_all_rays(t_global *global)
{
	int			i;
	int			total_pixels;
	t_vector	ray_dir;

	total_pixels = (WIN_W - MARGIN) * (WIN_H - MARGIN);
	i = -1;
	while (++i < total_pixels)
	{
		ray_dir.x = global->points[i].point_x;
		ray_dir.y = global->points[i].point_y;
		ray_dir.z = global->points[i].point_z;
		global->current_ray_origin = global->scene.cam.pos;
		global->current_ray_dir = ray_dir;
		global->intersecs[i] = find_closest_intersec(global);
	}
}

t_intersec	cal_ray(t_global *global, int px_x, int px_y)
{
	global->current_ray_origin = global->scene.cam.pos;
	global->current_ray_dir = get_ray_direction(global, px_x, px_y);
	return (find_closest_intersec(global));
}

/* Here we define the environment, axis directions,
	universe center (camera/observer),
   and screen relationships to take advantage of the entire view */
t_vector	get_ray_direction(t_global *global, int px_x, int px_y)
{
	static float	aspect_ratio = 0;
	static float	scrn_w = 0;
	static float	scrn_h = 0;
	static float	last_fov = -1;
	t_vector		ray_dir;
	t_camera		cam;

	float u, v;
	cam = global->scene.cam;
	if (last_fov != cam.fov)
	{
		aspect_ratio = (float)(WIN_W - MARGIN) / (float)(WIN_H - MARGIN);
		scrn_w = 2.0 * DSCR * tan((cam.fov * PI / 180.0) / 2.0);
		scrn_h = scrn_w / aspect_ratio;
		last_fov = cam.fov;
	}
	u = (2 * ((px_x + 0.5) / (WIN_W - MARGIN)) - 1) * scrn_w / 2;
	v = (2 * ((px_y + 0.5) / (WIN_H - MARGIN)) - 1) * scrn_h / 2;
	ray_dir = normalize(add(add(multiply(cam.x, u), multiply(cam.y, v)),
				cam.z));
	return (ray_dir);
}

t_vector	get_sp_normal(t_global *global, int sp_id, t_vector point)
{
	t_sphere	sphere;

	sphere = global->scene.spheres[sp_id];
	return (normalize(subtract(point, sphere.center)));
}

t_vector	get_pl_normal(t_global *global, int pl_id)
{
	if (pl_id < 0 || pl_id >= global->scene.num_pl)
		return ((t_vector){0, 1, 0});
	return (normalize(global->scene.planes[pl_id].normal));
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
	int			obj_type;
	int			obj_index;
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
	t_vector	axis;
	float		hit_height;
	t_vector	center_at_height;

	obj_type = intersec.obj_type;
	obj_index = intersec.obj_index;
	if (obj_type < 0 || obj_index < 0 || (obj_type == 0
			&& obj_index >= global->scene.num_sp) || (obj_type == 1
			&& obj_index >= global->scene.num_pl) || (obj_type == 2
			&& obj_index >= global->scene.num_cy))
	{
		return ((t_vector){0, 1, 0});
	}
	if (obj_type == 0)
	{
		sphere = global->scene.spheres[obj_index];
		return (normalize(subtract(intersec.point, sphere.center)));
	}
	else if (obj_type == 1)
	{
		plane = global->scene.planes[obj_index];
		return (normalize(plane.normal));
	}
	else
	{
		cylinder = global->scene.cylinders[obj_index];
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
}

void	check_obj_intersecs(t_global *global, t_intersec *closest_intersec,
		int obj_type)
{
	int			i;
	int			max_objs;
	t_intersec	temp_intersec;

	i = -1;
	if (obj_type == 0)
		max_objs = global->scene.num_sp;
	else if (obj_type == 1)
		max_objs = global->scene.num_pl;
	else
		max_objs = global->scene.num_cy;
	while (++i < max_objs)
	{
		if (obj_type == 0)
			temp_intersec = col_sp(global, i);
		else if (obj_type == 1)
			temp_intersec = col_pl(global, i);
		else
			temp_intersec = col_cy(global, i);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = obj_type;
		}
	}
}
