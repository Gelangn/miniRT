// incluir cabecera 42

#include "../inc/minirt.h"

/* static int	counter_words(char **str)
{
	int	len;

	len = 0;
	while (str[len] != NULL)
		len++;
	return (len);
} */

/* static void	get_scene(t_scene *scene)
{
	int	i;

	i = 0;
	scene->fd = open(scene->file_path, O_RDONLY);
	if (scene->fd == (-1))
		finish(ERR_OPEN);
	scene->lines = (char **)malloc((scene->height + 1) * sizeof(char *));
	if (scene->lines == NULL)
	{
		close(scene->fd);
		finish(ERR_MALLOC);
	}
	while (i <= scene->height)
	{
		scene->lines[i] = get_next_line(scene->fd);
		ft_printf("%s", scene->lines[i]);
		i++;
	}
	sleep(10);
	ft_printf("\nscene readed!!\n");
	close(scene->fd);
} */
/* static void	get_scene_size(t_scene *scene)
{
	char	*line;
	char	**split;

	scene->height = 0;
	scene->width = 0;
	line = NULL;
	split = NULL;
	scene->fd = open(scene->file_path, O_RDONLY);
	if (scene->fd == (-1))
		finish(ERR_OPEN);
	line = get_next_line(scene->fd);
	if (line == NULL)
		finish(ERR_OPEN);
	split = ft_split(line, ' ');
	scene->width = counter_words(split);
	free(line);
	while (line != NULL)
	{
		line = get_next_line(scene->fd);
		free(line);
		scene->height++;
	}
	if (split != NULL)
		dbl_free(split);
	close(scene->fd);
} */

/* static void	fill_points(t_scene *scene)
{
	int	i;
	int	j;
	int	index;

	i = 0;
	j = 0;
	index = 0;
	scene->points = malloc(((scene->width * scene->height) + 1) * sizeof(t_point));
	while (j < scene->height && scene->lines[j] != NULL)
	{
		while (i < scene->width)
		{
			scene->points[index].point_x = i;
			scene->points[index].point_y = j;
			splited(scene, j, i, index);
			index++;
			i++;
		}
		free(scene->lines[j]);
		i = 0;
		j++;
	}
	free(scene->lines);
} */

