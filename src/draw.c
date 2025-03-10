// incluir cabecera 42

#include "../inc/minirt.h"

// for each point in the map we draw the horizontal and vertical lines
static void	draw_map(t_global *global)
{
	int	index;

	index = 0;
	while (index < global->map.nr_elems)
	{
		if (index % global->map.width != global->map.width - 1)
			//right_line(global, index);
		if (index < global->map.nr_elems - global->map.width)
			//down_line(global, index);
		index++;
	}
	free(global->map.points);
}

void	render(t_global *global)
{
	points_3d_to_2d(&global->map);
	scale_map(&global->map);
	draw_map(global);
}

void	pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + ((y * data->line_length) + x * (data->bits_per_pixel
				/ 8));
	*(unsigned int *)dst = color;
}
