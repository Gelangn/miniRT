// incluir cabecera 42

#include "../inc/minirt.h"

void	free_map(t_map *map)
{
	int	i;

	i = 0;
	while (map->lines[i])
	{
		free(map->lines[i]);
		i++;
	}
	free(map->lines);
}

void	free_global(t_global *global)
{
	free_map(&global->map);
	free(global->img.img);
	free(global->vars.mlx_win);
	free(global->vars.mlx_conn);
}

void	dbl_free(char **ptr)
{
	int	i;

	if (!ptr)
		return ;
	i = 0;
	while (ptr[i])
	{
		free(ptr[i]);
		i++;
	}
	free(ptr);
}
