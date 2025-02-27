#include "../inc/minirt.h"

static void	get_map(t_map *map)
{
	int i;

	i = 0;
	map->fd = open(map->arg_path, O_RDONLY);
	if (map->fd == (-1))
		finish(ERR_OPEN);
	map->lines = (char **)malloc((map->height + 1) * sizeof(char *));
	if (map->lines == NULL)
	{
		close(map->fd);
		finish(ERR_MALLOC);
	}
	while (i <= map->height)
	{
		map->lines[i] = get_next_line(map->fd);
		ft_printf("%s", map->lines[i]);
		i++;
	}
	ft_printf("\nmap readed!!\n");
	close(map->fd);
}