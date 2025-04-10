// incluir cabecera 42

#include "../inc/minirt.h"

void	render_pixel(t_global *global, t_intersec intersec, t_img *img, int x,
		int y)
{
	int			color;
	t_vector	normal;
	float		dot_normal_axis;

	if (intersec.obj_type == 0 && intersec.obj_index >= 0)
		color = rgb_to_int(global->scene.spheres[intersec.obj_index].color);
	else if (intersec.obj_type == 1 && intersec.obj_index >= 0)
		color = rgb_to_int(global->scene.planes[intersec.obj_index].color);
	else if (intersec.obj_type == 2 && intersec.obj_index >= 0)
	{
		normal = get_surface_normal(global, intersec);
		dot_normal_axis = fabs(dot(normal,
									global->scene.cylinders[intersec.obj_index].orientation));
		if (dot_normal_axis > 0.99)
			color = RED;
		else
			color = CYAN;
	}
	else
		color = DARK_GREY;
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
	cal_all_intersecs(global, intersecs);
	render_all_pixels(global, intersecs);
	free(intersecs);
}
