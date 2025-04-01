// incluir cabecera 42

#include "../inc/minirt.h"

float	calculate_discriminant(t_vector oc, t_vector ray_dir, float radius)
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

	intersec.distance = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	oc = subtract(ray_origin, sphere->center);
	discriminant = calculate_discriminant(oc, ray_dir, sphere->radius);
	if (discriminant < 0)
		return (intersec);
	t1 = (-dot(ray_dir, oc) - sqrt(discriminant)) / dot(ray_dir, ray_dir);
	t2 = (-dot(ray_dir, oc) + sqrt(discriminant)) / dot(ray_dir, ray_dir);
	if (t1 > 0 && t2 > 0)
		intersec.distance = fmin(t1, t2);
	else if (t1 > 0)
		intersec.distance = t1;
	else if (t2 > 0)
		intersec.distance = t2;
	else
		return (intersec);
	intersec.point = add(ray_origin, multiply(ray_dir, intersec.distance));
	return (intersec);
}

t_vector	get_ray_direction(t_camera camera, int pixel_x, int pixel_y)
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
	scrn_w = 2.0 * tan((camera.fov * PI / 180.0) / 2.0);
	scrn_h = scrn_w / aspect_ratio;
	forward = normalize(camera.orientation);
	right = normalize(cross((t_vector){0, 1, 0}, forward));
	up = cross(forward, right);
	u = (2 * ((pixel_x + 0.5) / WIN_W) - 1) * scrn_w / 2;
	v = (1 - 2 * ((pixel_y + 0.5) / WIN_H)) * scrn_h / 2;
	ray_dir = normalize(add(add(multiply(right, u), multiply(up, v)), forward));
	return (ray_dir);
}

t_intersec	find_closest_intersec(t_global *global, t_vector ray_origin,
		t_vector ray_dir)
{
	t_sphere	*sphere;
	//t_plane		*plane;
	//t_cylinder	*cylinder;
	t_intersec	temp_intersec;
	t_intersec	closest_intersec;
	int			i;

	closest_intersec.distance = INFINITY;
	closest_intersec.point = (t_vector){0, 0, 0};
	sphere = global->scene.spheres;
	//plane = global->scene.planes;
	//cylinder = global->scene.cylinders;
	i = 0;
	while (i < global->scene.num_spheres)
	{
		temp_intersec = col_sp(&sphere[i], ray_origin, ray_dir);
		if (temp_intersec.distance < closest_intersec.distance)
			closest_intersec = temp_intersec;
		i++;
	}
	/* // Verificamos los planos con while
	i = 0; // Reiniciamos el contador para los planos
	while (i < global->scene.num_planes)
	{
		temp_intersec = intersect_plane(&plane[i], ray_origin, ray_dir);
		if (temp_intersec.distance < closest_intersec.distance)
			closest_intersec = temp_intersec;
		i++; // Aumentamos el contador
	}
	// Verificamos los cilindros con while
	i = 0; // Reiniciamos el contador para los cilindros
	while (i < global->scene.num_cylinders)
	{
		temp_intersec = intersect_cylinder(&cylinder[i], ray_origin, ray_dir);
		if (temp_intersec.distance < closest_intersec.distance)
			closest_intersec = temp_intersec;
		i++; // Aumentamos el contador
	} */
	return (closest_intersec);
}

t_vector	render_pixel(t_global *global, int pixel_x, int pixel_y)
{
	t_vector	ray_dir;
	t_vector	hit_point;
	t_camera	camera;
	t_sphere	*sphere;
	t_intersec	intersec;

	camera = global->scene.camera;
	sphere = global->scene.spheres;
	ray_dir = get_ray_direction(camera, pixel_x, pixel_y);
	intersec = col_sp(sphere, camera.position, ray_dir);
	hit_point = intersec.point;
	if (magnitude(hit_point) > 0)
		pixel_put(&global->img, pixel_x, pixel_y, rgb_to_int(sphere->color));
	printf("Ray Dir: %f %f %f | Hit: %f %f %f\n", ray_dir.x, ray_dir.y,
		ray_dir.z, hit_point.x, hit_point.y, hit_point.z);
	return (hit_point);
}

void	render(t_global *global)
{
	int	x;
	int	y;
	int	i;

	x = -1;
	y = -1;
	i = 0;
	while (++x < WIN_W)
	{
		y = -1;
		while (++y < WIN_H)
		{
			i++;
			render_pixel(global, x, y);
		}
	}
	printf("Total rays: %d\n", i); // Verificar conteo final
}
