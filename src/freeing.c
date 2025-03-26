// incluir cabecera 42

#include "../inc/minirt.h"

void	free_spheres(t_sphere *spheres)
{
	if (spheres)
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
}

void	free_global(t_global *global)
{
	if (global->vars.mlx_win)
	{
		mlx_destroy_window(global->vars.mlx_conn, global->vars.mlx_win);
		// Destruir la ventana
	}
	if (global->vars.mlx_conn)
	{
		mlx_destroy_display(global->vars.mlx_conn);
		// Destruir la conexión de display
	}
	free_scene(&global->scene);
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
