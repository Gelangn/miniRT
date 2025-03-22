// incluir cabecera 42

#include "../inc/minirt.h"

void free_scene(t_scene *scene)
{
	while (scene->lines)
	{
		if (scene->lines)
		{
			free(scene->lines);
			scene->lines++;
		}
	}
	if (scene->spheres)
		free(scene->spheres);
	if (scene->planes)
		free(scene->planes);
	if (scene->cylinders)
		free(scene->cylinders);
	free(scene);
}

void free_global(t_global *global)
{
	free_scene(&global->scene);
	free(global->img.img);
	free(global->vars.mlx_win);
	free(global->vars.mlx_conn);
}
// si no se usa se puede borrar
void dbl_free(char **ptr)
{
	if (!ptr)
		return;
	while (ptr)
	{
		free(*ptr);
		ptr++;
	}
	free(ptr);
}
