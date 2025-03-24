// incluir cabecera 42

#include "../inc/minirt.h"

void	free_spheres(t_sphere *spheres)
{
	free(spheres);
}

void	free_planes(t_plane *planes)
{
	free(planes);
}

void	free_cylinders(t_cylinder *cylinders)
{
	free(cylinders);
}

void	free_scene(t_scene *scene)
{
	char	**lines;

	if (scene->spheres)
		free_spheres(scene->spheres);
	if (scene->planes)
		free_planes(scene->planes);
	if (scene->cylinders)
		free_cylinders(scene->cylinders);
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
	if (scene->file_path)
		free(scene->file_path);
	// No liberar scene ya que no fue asignado dinámicamente aquí
}
void	free_global(t_global *global)
{
	free_scene(&global->scene);
	if (global->vars.mlx_win)
		free(global->vars.mlx_win);
	if (global->vars.mlx_conn)
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
