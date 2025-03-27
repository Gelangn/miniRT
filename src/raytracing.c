// incluir cabecera 42

#include "../inc/minirt.h"

static t_vector	ray(t_vector camera, int pixel_x, int pixel_y)
{
	float		x;
	t_vector	scrn_pnt;
	t_vector	ray_dir;
	float		magnitud;
	t_vector	n_ray_dir;

	/* Calcula la dirección de un rayo desde la cámara hasta un píxel en la pantalla virtual. */
	// Coordenadas del píxel en el espacio 3D de la pantalla virtual
	x = (pixel_x - WIN_W / 2.0f);
	float y = -(pixel_y - WIN_H / 2.0f); // Invertir el eje Y
	float z = -DSCR;                     // La pantalla está frente a la cámara
	scrn_pnt = (t_vector){x, y, z};
	// Dirección del rayo: vector desde la cámara al píxel
	ray_dir = (t_vector){scrn_pnt.x - camera.x, scrn_pnt.y - camera.y,
		scrn_pnt.z - camera.z};
	// Normalizar la dirección del rayo
	magnitud = sqrt(ray_dir.x * ray_dir.x + ray_dir.y * ray_dir.y + ray_dir.z
			* ray_dir.z);
	n_ray_dir = (t_vector){ray_dir.x / magnitud, ray_dir.y / magnitud, ray_dir.z
		/ magnitud};
	return (n_ray_dir);
}

t_vector	calc_ray(t_camera *camera)
{
	t_vector result;
	int x = -1;
	int y = -1;
	int i;

	i = 0;
	while (++x < WIN_W)
	{
		y = -1;
		while (++y < WIN_H)
		{
			i++;
			result = ray(camera->orientation, x, y);
			printf("Ray %d: %f %f %f\n", i, result.x, result.y, result.z);
		}
	}
	printf("Total rays: %d\n", i); // Verificar conteo final
	return (result);
}