// incluir cabecera 42

#include "../inc/minirt.h"

void	render_pixel(t_global *global, t_intersec intersec, t_img *img, int x,
		int y)
{
	int			color;
	t_vector	ray_dir;
	t_color		lit_color;

	if (intersec.obj_type >= 0 && intersec.obj_index >= 0)
	{
		// Recalcular dirección del rayo para usarla en la iluminación
		ray_dir = get_ray_direction(global->scene.cam, x, y);
		// Calcular color con iluminación completa
		lit_color = cal_lighting(global, intersec, ray_dir);
		color = rgb_to_int(lit_color);
	}
	else
	{
		// Fondo
		color = BLACK;
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
	while (++x < WIN_W)
	{
		y = -1;
		while (++y < WIN_H)
		{
			render_pixel(global, intersecs[i++], &global->img, x, y);
		}
	}
}

void	render(t_global *global)
{
	t_intersec	*intersecs;

	intersecs = malloc(WIN_W * WIN_H * sizeof(t_intersec));
	if (!intersecs)
		return ;
	trace_all_rays(global, intersecs);
	render_all_pixels(global, intersecs);
	free(intersecs);
}
