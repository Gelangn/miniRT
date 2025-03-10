// incluir cabecera 42

#include "../inc/minirt.h"

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