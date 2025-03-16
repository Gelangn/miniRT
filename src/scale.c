// incluir cabecera 42

#include "../inc/minirt.h"

float	max_range_w(t_scene *scene)
{
	int		index;
	float	max_x;

	index = 0;
	max_x = 0.0;
	max_x = scene->points[index].scrn_x;
	while (index < scene->nr_elems)
	{
		if (scene->points[index].scrn_x < scene->points[index + 1].scrn_x
			&& scene->points[index + 1].scrn_x > max_x)
			max_x = scene->points[index + 1].scrn_x;
		index++;
	}
	return (max_x);
}

float	min_range_w(t_scene *scene)
{
	int		index;
	float	min_x;

	index = 0;
	min_x = 0.0;
	min_x = scene->points[index].scrn_x;
	while (index < scene->nr_elems)
	{
		if (scene->points[index].scrn_x > scene->points[index + 1].scrn_x
			&& scene->points[index + 1].scrn_x < min_x)
			min_x = scene->points[index + 1].scrn_x;
		index++;
	}
	return (min_x);
}

float	max_range_h(t_scene *scene)
{
	int		index;
	float	max_y;

	index = 0;
	max_y = 0.0;
	max_y = scene->points[index].scrn_y;
	while (index < scene->nr_elems)
	{
		if (scene->points[index].scrn_y < scene->points[index + 1].scrn_y
			&& scene->points[index + 1].scrn_y > max_y)
			max_y = scene->points[index + 1].scrn_y;
		index++;
	}
	return (max_y);
}

float	min_range_h(t_scene *scene)
{
	int		index;
	float	min_y;

	index = 0;
	min_y = 0.0;
	min_y = scene->points[index].scrn_y;
	while (index < scene->nr_elems)
	{
		if (scene->points[index].scrn_y > scene->points[index + 1].scrn_y
			&& scene->points[index + 1].scrn_y < min_y)
			min_y = scene->points[index + 1].scrn_y;
		index++;
	}
	return (min_y);
}

void	scale_scene(t_scene *scene)
{
	float range_x;
	float range_y;

	range_x = 0;
	range_y = 0;
	range_x = fabsf(max_range_w(scene)) + fabsf(min_range_w(scene));
	range_y = fabsf(max_range_h(scene)) + fabsf(min_range_h(scene));
	if (((WIN_H - MARGIN) / range_y) > ((WIN_W - MARGIN) / range_x))
		scene->scale = (((WIN_W - MARGIN) / range_x)) * (0.9);
	scene->scale = (((WIN_H - MARGIN) / range_y)) * (0.9);
	scene->origin_x = (((WIN_W - MARGIN) - (range_x * scene->scale)) / 2)
		+ fabsf(min_range_w(scene) * scene->scale);
	scene->origin_y = (((WIN_H - MARGIN) - (range_y * scene->scale)) / 2)
		+ fabsf(min_range_h(scene) * scene->scale);
}