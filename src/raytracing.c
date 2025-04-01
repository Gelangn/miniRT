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

t_vector	col_sp(t_sphere *sphere, t_vector ray_origin, t_vector ray_dir)
{
	float		t1;
	float		t2;
	float		discriminant;
	t_vector	oc;

	oc = subtract(ray_origin, sphere->center);
	discriminant = calculate_discriminant(oc, ray_dir, sphere->radius);
	if (discriminant < 0)
		return ((t_vector){0, 0, 0});
	t1 = (-dot(ray_dir, oc) - sqrt(discriminant)) / dot(ray_dir, ray_dir);
	t2 = (-dot(ray_dir, oc) + sqrt(discriminant)) / dot(ray_dir, ray_dir);
	if (t1 > 0 && t2 > 0)
		return (add(ray_origin, multiply(ray_dir, fmin(t1, t2))));
	else if (t1 > 0)
		return (add(ray_origin, multiply(ray_dir, t1)));
	else if (t2 > 0)
		return (add(ray_origin, multiply(ray_dir, t2)));
	else
		return ((t_vector){0, 0, 0});
	return ((t_vector){0, 0, 0});
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

t_vector	render_pixel(t_global *global, int pixel_x, int pixel_y)
{
	t_vector	ray_dir;
	t_vector	hit_point;
	t_camera	camera;
	t_sphere	*sphere;

	camera = global->scene.camera;
	sphere = global->scene.spheres;
	ray_dir = get_ray_direction(camera, pixel_x, pixel_y);
	hit_point = col_sp(sphere, camera.position, ray_dir);
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
