// incluir cabecera 42

#include "../inc/minirt.h"

t_vector	col_sp(t_sphere *sphere, t_vector ray_origin, t_vector ray_dir)
{
	float		a;
	float		b;
	float		c;
	float		t1;
	float		t2;
	float		discriminant;
	t_vector	oc;
	t_vector	result;

	oc = subtract(ray_origin, sphere->center);
	a = dot(ray_dir, ray_dir);
	b = 2.0 * dot(oc, ray_dir);
	c = dot(oc, oc) - sphere->radius * sphere->radius;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		result = (t_vector){0, 0, 0}; // No collision
	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	t2 = (-b + sqrt(discriminant)) / (2.0 * a);
	if (t1 > 0)
		result = add(ray_origin, multiply(ray_dir, t1));
	else if (t2 > 0)
		result = add(ray_origin, multiply(ray_dir, t2));
	else
		result = (t_vector){0, 0, 0}; // No collision
	return (result);
}

// Function to calculate the direction of a ray from the camera to a pixel on the virtual screen.
t_vector	ray(t_global global, int pixel_x, int pixel_y)
{
	float		x;
	float		y;
	float		z;
	t_vector	scrn_pnt;
	t_vector	camera;
	t_vector	hit_point;

	camera = global.scene.camera.position;
	t_vector ray_dir;              // Ray direction
	t_vector n_ray_dir;            // Normalized ray direction
	x = (pixel_x - WIN_W / 2.0f);  // Centrar el eje X
	y = -(pixel_y - WIN_H / 2.0f); // Centra el eje Y y lo invierte
	z = -DSCR;                     // La pantalla está frente a la cámara
	scrn_pnt = (t_vector){x, y, z};
	// Dirección del rayo: vector desde la cámara al píxel
	ray_dir = (t_vector){scrn_pnt.x - camera.x, scrn_pnt.y - camera.y,
		scrn_pnt.z - camera.z};
	// Normalizar la dirección del rayo
	n_ray_dir = normalize(ray_dir);
	hit_point = col_sp(global.scene.spheres, camera, n_ray_dir);
	// Verificar si el rayo colisiona con la esfera
	if (magnitude(hit_point) > 0)
		pixel_put(&global.img, pixel_x, pixel_y, rgb_to_int(global.scene.spheres->color));
	return (n_ray_dir);
}

t_vector	calc_ray(t_global *global)
{
	t_vector result;
	int x = -1;
	int y = -1;
	int i;
	// t_camera *camera = &global->scene.camera;

	i = 0;
	while (++x < WIN_W)
	{
		y = -1;
		while (++y < WIN_H)
		{
			i++;
			result = ray(*global, x, y);
			printf("Ray %d: %f %f %f\n", i, result.x, result.y, result.z);
		}
	}
	printf("Total rays: %d\n", i); // Verificar conteo final
	return (result);
}