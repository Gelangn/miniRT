// incluir cabecera 42

#include "../inc/minirt.h"

void	free_scene(t_scene *scene)
{
	char	**lines;

	if (scene->spheres)
		free(scene->spheres);
	if (scene->planes)
		free(scene->planes);
	if (scene->cylinders)
		free(scene->cylinders);
	if (scene->lines)
	{
		lines = scene->lines;
		while (*lines)
		{
			free(*lines);
			lines++;
		}
		free(scene->lines);
	}
}

void	free_global(t_global *global)
{
	// Liberar los puntos precalculados
	if (global->points)
		free(global->points);
	// Liberar los recursos de la escena
	free_scene(&global->scene);
	// Luego liberar MLX en el orden correcto
	if (global->img.img)
		mlx_destroy_image(global->vars.mlx_conn, global->img.img);
	if (global->vars.mlx_win)
		mlx_destroy_window(global->vars.mlx_conn, global->vars.mlx_win);
	if (global->vars.mlx_conn)
	{
		mlx_destroy_display(global->vars.mlx_conn);
		free(global->vars.mlx_conn);
	}
	// Finalmente liberar la estructura global
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
