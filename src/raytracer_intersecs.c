/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_intersecs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:59:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 21:08:54 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

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

t_intersec	find_closest_intersec(t_global *global)
{
	t_intersec	closest_intersec;

	closest_intersec = init_intersec();
	if (global->scene.num_sp > 0)
		check_obj_intersecs(global, &closest_intersec, 0);
	if (global->scene.num_pl > 0)
		check_obj_intersecs(global, &closest_intersec, 1);
	if (global->scene.num_cy > 0)
		check_obj_intersecs(global, &closest_intersec, 2);
	return (closest_intersec);
}

int	is_valid_intersec(t_global *global)
{
	t_intersec	intersec;

	intersec = global->current_intersec;
	if ((intersec.obj_type < 0) || (intersec.obj_index < 0)
		|| (intersec.obj_type == 0
			&& intersec.obj_index >= global->scene.num_sp)
		|| (intersec.obj_type == 1
			&& intersec.obj_index >= global->scene.num_pl)
		|| (intersec.obj_type == 2
			&& intersec.obj_index >= global->scene.num_cy))
	{
		return (0);
	}
	return (1);
}
