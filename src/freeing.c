/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:15:32 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 12:27:29 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	finish(t_global *global, const char *message)
{
	if (errno == 0)
		ft_putendl_fd((char *)message, 2);
	else
		perror(message);
	if (global)
		free_global(global);
	printf("Exiting program\n");
	exit(SUCCESS);
}

void	free_scene(t_scene *scene)
{
	char	**lines;

	if (scene->spheres)
		free(scene->spheres);
	if (scene->planes)
		free(scene->planes);
	if (scene->cyls)
		free(scene->cyls);
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
	if (global->points)
		free(global->points);
	free_scene(&global->scene);
	if (global->img.img)
		mlx_destroy_image(global->vars.mlx_conn, global->img.img);
	if (global->vars.mlx_win)
		mlx_destroy_window(global->vars.mlx_conn, global->vars.mlx_win);
	if (global->vars.mlx_conn)
	{
		mlx_destroy_display(global->vars.mlx_conn);
		free(global->vars.mlx_conn);
	}
}

/**
 * Safely frees a double pointer array and sets it to NULL
 * Prevents double free errors by checking validity before freeing
 * 
 * @param ptr Double pointer to the array to be freed
 */
void	dbl_free(char **ptr)
{
	int	i;

	if (!ptr)
		return ;
	i = 0;
	while (ptr[i])
	{
		if (ptr[i])
		{
			free(ptr[i]);
			ptr[i] = NULL;
		}
		i++;
	}
	free(ptr);
	ptr = NULL;
}
