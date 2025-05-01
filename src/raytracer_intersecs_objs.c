/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_intersecs_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:11:59 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/02 00:15:53 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	check_sp_intersecs(t_global *global, t_intersec *closest_intersec)
{
	int			i;
	t_intersec	temp_intersec;

	i = -1;
	while (++i < global->scene.num_sp)
	{
		temp_intersec = col_sp(global, i);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = 0;
		}
	}
}

void	check_pl_intersecs(t_global *global, t_intersec *closest_intersec)
{
	int			i;
	t_intersec	temp_intersec;

	i = -1;
	while (++i < global->scene.num_pl)
	{
		temp_intersec = col_pl(global, i);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = 1;
		}
	}
}

void	check_cy_intersecs(t_global *global, t_intersec *closest_intersec)
{
	int			i;
	t_intersec	temp_intersec;

	i = -1;
	while (++i < global->scene.num_cy)
	{
		temp_intersec = col_cy(global, i);
		if (temp_intersec.dist < closest_intersec->dist)
		{
			*closest_intersec = temp_intersec;
			closest_intersec->obj_index = i;
			closest_intersec->obj_type = 2;
		}
	}
}
