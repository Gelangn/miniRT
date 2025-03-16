// incluir cabecera 42

#include "../inc/minirt.h"

// for each point in the scene we draw the horizontal and vertical lines
static void	draw_scene(t_global *global)
{
	int	index;

	index = 0;
	while (index < global->scene.nr_elems)
	{
		if (index % global->scene.width != global->scene.width - 1)
			//right_line(global, index);
		if (index < global->scene.nr_elems - global->scene.width)
			//down_line(global, index);
		index++;
	}
	free(global->scene.points);
}

void	render(t_global *global)
{
	points_3d_to_2d(&global->scene);
	scale_scene(&global->scene);
	draw_scene(global);
}

void	pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + ((y * data->line_length) + x * (data->bits_per_pixel
				/ 8));
	*(unsigned int *)dst = color;
}
