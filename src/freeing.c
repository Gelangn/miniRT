// incluir cabecera 42

#include "../inc/minirt.h"

void	free_scene(t_scene *scene)
{
	char	**lines;

	if (scene->lines)
	{
		lines = scene->lines;
		while (*lines)
		{
			free(*lines);
			lines++;
		}
		free(scene->lines); // Liberar el array de punteros
	}
	if (scene->spheres)
		free(scene->spheres);
	if (scene->planes)
		free(scene->planes);
	if (scene->cylinders)
		free(scene->cylinders);
	// No liberar scene ya que no fue asignado dinámicamente aquí
}
void	free_global(t_global *global)
{
	free_scene(&global->scene);
	if (global->vars.mlx_conn)
		mlx_destroy_display(global->vars.mlx_conn);
	free(global->vars.mlx_conn);
	free(global);
}
void	dbl_free(char **ptr)
{
	char	**temp;

	if (!ptr)
		return ;
	temp = ptr;
	while (*temp)
	{
		free(*temp);
		temp++;
	}
	free(ptr);
}
