// incluir cabecera 42

#include "../inc/minirt.h"


// Function to calculate the direction of a ray from the camera to a pixel on the virtual screen.
static t_vector	ray(t_vector camera, int pixel_x, int pixel_y)
{
	float		x;
	float		y;
	float		z;
	t_vector	scrn_pnt; 
	t_vector	ray_dir; // Ray direction
	t_vector	n_ray_dir; // Normalized ray direction

	x = (pixel_x - WIN_W / 2.0f);	// Centrar el eje X
	y = -(pixel_y - WIN_H / 2.0f);	// Centra el eje Y y lo invierte
	z = -DSCR;						// La pantalla está frente a la cámara
	scrn_pnt = (t_vector){x, y, z};
	// Dirección del rayo: vector desde la cámara al píxel
	ray_dir = (t_vector){scrn_pnt.x - camera.x, scrn_pnt.y - camera.y,
		scrn_pnt.z - camera.z};
	// Normalizar la dirección del rayo
	n_ray_dir = normalize(ray_dir);
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
			//colision(result);
			printf("Ray %d: %f %f %f\n", i, result.x, result.y, result.z);
		}
	}
	printf("Total rays: %d\n", i); // Verificar conteo final
	return (result);
}