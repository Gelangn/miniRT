/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-mada <bde-mada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:15:32 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/18 19:22:15 by bde-mada         ###   ########.fr       */
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

void	dbl_free(char **ptr)
{
	char	**temp;

	if (!ptr)
		return ;
	temp = ptr;
	while (*temp)
	{
		free(*temp);
		*temp = NULL;
		temp++;
	}
	free(ptr);
	ptr = NULL;
}
