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
	int	x;
	int	y;
	int	i;

	i = 0;
	x = -1;
	// Solo renderizar para el tamaño de la imagen
	while (++x < (WIN_W - MARGIN))
	{
		y = -1;
		while (++y < (WIN_H - MARGIN))
		{
			render_pixel(global, intersecs[i++], &global->img, x, y);
		}
	}
}

void	render(t_global *global)
{
	t_intersec	*intersecs;

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
	trace_all_rays(global, intersecs);
	render_all_pixels(global, intersecs);
	free(intersecs);
}
