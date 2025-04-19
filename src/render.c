// incluir cabecera 42

#include "../inc/minirt.h"

void	render_pixel(t_global *global, t_intersec intersec, t_img *img, int x,
		int y)
{
	int		color;
	t_color	lit_color;

	// Verificar límites
	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H || !img || !img->addr)
		return ;
	if (intersec.obj_type >= 0 && intersec.obj_index >= 0)
	{
		global->current_ray_dir = get_ray_direction(global->scene.cam, x, y);
		global->current_intersec = intersec;
		lit_color = cal_lighting(global);
		color = rgb_to_int(lit_color);
	}
	else
	{
		color = DARK_GREY;
	}
	pixel_put(img, x, y, color);
}

void	render_all_pixels(t_global *global, t_intersec *intersecs)
{
	int	i;
	int	px_x;
	int	px_y;

	i = -1;
	while (++i < (WIN_W - MARGIN) * (WIN_H - MARGIN))
	{
		// Usar las coordenadas precalculadas
		px_x = global->points[i].scrn_x;
		px_y = global->points[i].scrn_y;
		// Verificar el píxel central para debug
		if (px_x == WIN_W / 2 && px_y == WIN_H / 2)
		{
			printf("Central ray: type=%d, index=%d, distance=%f\n",
				intersecs[i].obj_type, intersecs[i].obj_index,
				intersecs[i].dist);
		}
		render_pixel(global, intersecs[i], &global->img, px_x, px_y);
	}
}

void	render(t_global *global)
{
	t_intersec	*intersecs;

	// Verificar orientación válida
	if (comp_floats(magnitude(global->scene.cam.orientation), 0))
	{
		// Usar una dirección por defecto
		global->scene.cam.orientation = (t_vector){0, 0, 1};
	}
	// Precalcular los rayos
	precalculate_rays(global);
	// Asignar memoria para las intersecciones
	intersecs = malloc((WIN_W - MARGIN) * (WIN_H - MARGIN)
			* sizeof(t_intersec));
	if (!intersecs)
		finish(global, ERR_MEM);
	// Verificar validez de la imagen
	if (!global->img.img || !global->img.addr)
	{
		free(intersecs);
		finish(global, ERR_IMG);
	}
	printf("Camera position: (%f, %f, %f)\n", global->scene.cam.pos.x,
		global->scene.cam.pos.y, global->scene.cam.pos.z);
	printf("Camera orientation: (%f, %f, %f)\n",
		global->scene.cam.orientation.x, global->scene.cam.orientation.y,
		global->scene.cam.orientation.z);
	// Calcular intersecciones
	trace_all_rays(global, intersecs);
	// Renderizar píxeles
	render_all_pixels(global, intersecs);
	// Liberar memoria
	free(intersecs);
}

void	precalculate_camera_axis(t_global *global)
{
	t_camera	*cam;

	cam = &global->scene.cam;
	// Calcular y almacenar los vectores base una sola vez
	cam->z = normalize(cam->orientation);
	cam->x = normalize(cross((t_vector){0, 1, 0}, cam->z));
	cam->y = normalize(cross(cam->z, cam->x));
}

void	precalculate_rays(t_global *global)
{
	int			i;
	int			px_x;
	int			px_y;
	t_vector	dir;

	// Liberar memoria si ya existía
	if (global->points)
		free(global->points);
	// Asignar memoria para todos los rayos
	global->points = malloc((WIN_W - MARGIN) * (WIN_H - MARGIN)
			* sizeof(t_point));
	if (!global->points)
		finish(global, ERR_MEM);
	// Primero precalcular los ejes de la cámara
	precalculate_camera_axis(global);
	// Calcular las direcciones de los rayos
	i = 0;
	px_x = -1;
	while (++px_x < (WIN_W - MARGIN))
	{
		px_y = -1;
		while (++px_y < (WIN_H - MARGIN))
		{
			// Guardar coordenadas
			global->points[i].scrn_x = px_x;
			global->points[i].scrn_y = px_y;
			// Precalcular y guardar la dirección del rayo
			dir = get_ray_direction(global->scene.cam, px_x, px_y);
			global->points[i].point_x = dir.x;
			global->points[i].point_y = dir.y;
			global->points[i].point_z = dir.z;
			i++;
		}
	}
}
