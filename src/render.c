// incluir cabecera 42

#include "../inc/minirt.h"

void	render_pixel(t_global *global, t_intersec intersec, t_img *img, int x,
		int y)
{
	int			color;
	t_vector	ray_dir;
	t_color		lit_color;

	// Verificar límites
	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H || !img || !img->addr)
		return ;
	if (intersec.obj_type >= 0 && intersec.obj_index >= 0)
	{
		ray_dir = get_ray_direction(global->scene.cam, x, y);
		lit_color = cal_lighting(global, intersec, ray_dir);
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
	int			x;
	int			y;
	int			i;
	t_vector	ray_dir;
	t_intersec	center_intersec;

	i = 0;
	x = -1;
	// Solo renderizar para el tamaño de la imagen
	while (++x < (WIN_W - MARGIN))
	{
		y = -1;
		while (++y < (WIN_H - MARGIN))
		{
			if (x == WIN_W / 2 && y == WIN_H / 2)
			{
				ray_dir = get_ray_direction(global->scene.cam, x, y);
				printf("Center ray direction: (%f, %f, %f)\n", ray_dir.x,
					ray_dir.y, ray_dir.z);
				center_intersec = find_closest_intersec(global,
						global->scene.cam.pos, ray_dir);
				printf("Center ray hit: type=%d, index=%d, distance=%f\n",
					center_intersec.obj_type, center_intersec.obj_index,
					center_intersec.dist);
			}
			render_pixel(global, intersecs[i++], &global->img, x, y);
		}
	}
}

void	render(t_global *global)
{
	t_intersec	*intersecs;

	// Si el vector de orientación es (0,0,0), asumimos que es un punto objetivo
	if (comp_floats(magnitude(global->scene.cam.orientation), 0))
	{
		// Usar el origen como punto objetivo
		global->scene.cam.orientation = subtract((t_vector){0, 0, 0},
				global->scene.cam.pos);
		global->scene.cam.orientation = normalize(global->scene.cam.orientation);
	}
	// Asignar memoria solo para los píxeles de la imagen
	intersecs = malloc((WIN_W - MARGIN) * (WIN_H - MARGIN)
			* sizeof(t_intersec));
	if (!intersecs)
		finish(global, ERR_MEM);
	// Verificar que la imagen existe y tiene una dirección válida
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
	trace_all_rays(global, intersecs);
	render_all_pixels(global, intersecs);
	free(intersecs);
}
