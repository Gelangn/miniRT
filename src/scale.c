// incluir cabecera 42

#include "../inc/minirt.h"

float	max_range_w(t_map *map)
{
	int		index;
	float	max_x;

	index = 0;
	max_x = 0.0;
	max_x = map->points[index].scrn_x;
	while (index < map->nr_elems)
	{
		if (map->points[index].scrn_x < map->points[index + 1].scrn_x
			&& map->points[index + 1].scrn_x > max_x)
			max_x = map->points[index + 1].scrn_x;
		index++;
	}
	return (max_x);
}

float	min_range_w(t_map *map)
{
	int		index;
	float	min_x;

	index = 0;
	min_x = 0.0;
	min_x = map->points[index].scrn_x;
	while (index < map->nr_elems)
	{
		if (map->points[index].scrn_x > map->points[index + 1].scrn_x
			&& map->points[index + 1].scrn_x < min_x)
			min_x = map->points[index + 1].scrn_x;
		index++;
	}
	return (min_x);
}

float	max_range_h(t_map *map)
{
	int		index;
	float	max_y;

	index = 0;
	max_y = 0.0;
	max_y = map->points[index].scrn_y;
	while (index < map->nr_elems)
	{
		if (map->points[index].scrn_y < map->points[index + 1].scrn_y
			&& map->points[index + 1].scrn_y > max_y)
			max_y = map->points[index + 1].scrn_y;
		index++;
	}
	return (max_y);
}

float	min_range_h(t_map *map)
{
	int		index;
	float	min_y;

	index = 0;
	min_y = 0.0;
	min_y = map->points[index].scrn_y;
	while (index < map->nr_elems)
	{
		if (map->points[index].scrn_y > map->points[index + 1].scrn_y
			&& map->points[index + 1].scrn_y < min_y)
			min_y = map->points[index + 1].scrn_y;
		index++;
	}
	return (min_y);
}

void	scale_map(t_map *map)
{
	float range_x;
	float range_y;

	range_x = 0;
	range_y = 0;
	range_x = fabsf(max_range_w(map)) + fabsf(min_range_w(map));
	range_y = fabsf(max_range_h(map)) + fabsf(min_range_h(map));
	if (((WIN_H - MARGIN) / range_y) > ((WIN_W - MARGIN) / range_x))
		map->scale = (((WIN_W - MARGIN) / range_x)) * (0.9);
	map->scale = (((WIN_H - MARGIN) / range_y)) * (0.9);
	map->origin_x = (((WIN_W - MARGIN) - (range_x * map->scale)) / 2)
		+ fabsf(min_range_w(map) * map->scale);
	map->origin_y = (((WIN_H - MARGIN) - (range_y * map->scale)) / 2)
		+ fabsf(min_range_h(map) * map->scale);
}