// incluir cabecera 42

#include "../inc/minirt.h"

// Función simplificada que aprovecha datos ya almacenados en global
void	render_single_pixel(t_global *global, int index)
{
	int			x;
	int			y;
	t_intersec	intersec;
	int			color;
	t_color		lit_color;

	x = global->points[index].scrn_x;
	y = global->points[index].scrn_y;
	intersec = global->intersecs[index];
	// Verificar límites (por seguridad)
	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	if (intersec.obj_type >= 0 && intersec.obj_index >= 0)
	{
		// Usar directamente la dirección del rayo precalculada
		global->current_ray_dir.x = global->points[index].point_x;
		global->current_ray_dir.y = global->points[index].point_y;
		global->current_ray_dir.z = global->points[index].point_z;
		global->current_intersec = intersec;
		// Calcular color con iluminación
		lit_color = cal_lighting(global);
		color = rgb_to_int(lit_color);
	}
	else
	{
		color = DARK_GREY;
	}
	pixel_put(&global->img, x, y, color);
}

// Función simplificada que itera sobre todos los píxeles
void	render_all_pixels(t_global *global)
{
	int	i;
	int	total_pixels;
	int	center_idx;

	total_pixels = (WIN_W - MARGIN) * (WIN_H - MARGIN);
	// Verificar el píxel central para debug
	center_idx = (WIN_H / 2) * (WIN_W - MARGIN) + (WIN_W / 2);
	if (center_idx < total_pixels)
	{
		printf("Central ray: type=%d, index=%d, distance=%f\n",
			global->intersecs[center_idx].obj_type,
			global->intersecs[center_idx].obj_index,
			global->intersecs[center_idx].dist);
	}
	// Renderizar todos los píxeles
	i = 0;
	while (i < total_pixels)
	{
		render_single_pixel(global, i);
		i++;
	}
}

// Función principal de renderizado simplificada
void	render(t_global *global)
{
	// Verificar orientación válida
	if (comp_floats(magnitude(global->scene.cam.orientation), 0))
	{
		// Usar una dirección por defecto
		global->scene.cam.orientation = (t_vector){0, 0, 1};
	}
	// Precalcular los rayos
	precalculate_rays(global);
	// Asignar memoria para las intersecciones
	global->intersecs = malloc((WIN_W - MARGIN) * (WIN_H - MARGIN)
			* sizeof(t_intersec));
	if (!global->intersecs)
		finish(global, ERR_MEM);
	// Verificar validez de la imagen
	if (!global->img.img || !global->img.addr)
	{
		free(global->intersecs);
		global->intersecs = NULL;
		finish(global, ERR_IMG);
	}
	printf("Camera position: (%f, %f, %f)\n", global->scene.cam.pos.x,
		global->scene.cam.pos.y, global->scene.cam.pos.z);
	printf("Camera orientation: (%f, %f, %f)\n",
		global->scene.cam.orientation.x, global->scene.cam.orientation.y,
		global->scene.cam.orientation.z);
	// Calcular intersecciones
	trace_all_rays(global);
	// Renderizar píxeles
	render_all_pixels(global);
	// Liberar memoria
	free(global->intersecs);
	global->intersecs = NULL;
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
