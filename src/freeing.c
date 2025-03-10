/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 13:31:43 by anavas-g          #+#    #+#             */
/*   Updated: 2025/03/10 21:01:02 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"
#include "../inc/struct.h"

void	free_map(t_map *map)
{
	int	i;

	i = 0;
	while (map->lines[i])
	{
		free(map->lines[i]);
		i++;
	}
	free(map->lines);
}

void	free_global(t_global *global)
{
	free_map(&global->map);
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
	free (ptr);
}
