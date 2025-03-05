#include "../inc/minirt.h"

static int	counter_words(char **str)
{
	int	len;

	len = 0;
	while (str[len] != NULL)
		len++;
	return (len);
}

static void	get_map(t_map *map)
{
	int	i;

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
static void	get_map_size(t_map *map)
{
	char	*line;
	char	**split;

	map->height = 0;
	map->width = 0;
	line = NULL;
	split = NULL;
	map->fd = open(map->arg_path, O_RDONLY);
	if (map->fd == (-1))
		finish(ERR_OPEN);
	line = get_next_line(map->fd);
	if (line == NULL)
		finish(ERR_OPEN);
	split = ft_split(line, ' ');
	map->width = counter_words(split);
	free(line);
	while (line != NULL)
	{
		line = get_next_line(map->fd);
		free(line);
		map->height++;
	}
	if (split != NULL)
		dbl_free(split);
	close(map->fd);
}

static void	fill_points(t_map *map)
{
	int	i;
	int	j;
	int	index;

	i = 0;
	j = 0;
	index = 0;
	map->points = malloc(((map->width * map->height) + 1) * sizeof(t_point));
	while (j < map->height && map->lines[j] != NULL)
	{
		while (i < map->width)
		{
			map->points[index].point_x = i;
			map->points[index].point_y = j;
			splited(map, j, i, index);
			index++;
			i++;
		}
		free(map->lines[j]);
		i = 0;
		j++;
	}
	free(map->lines);
}

t_map	read_map(t_map *map, char *arg_path)
{
	map->arg_path = arg_path;
	get_map_size(map);
	get_map(map);
	map->nr_elems = map->width * map->height;
	fill_points(map);
	return (*map);
}
