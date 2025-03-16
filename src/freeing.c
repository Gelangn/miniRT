// incluir cabecera 42

#include "../inc/minirt.h"

void	free_scene(t_scene *scene)
{
	int	i;

	i = 0;
	while (scene->lines[i])
	{
		free(scene->lines[i]);
		i++;
	}
	free(scene->lines);
}

void	free_global(t_global *global)
{
	free_scene(&global->scene);
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
