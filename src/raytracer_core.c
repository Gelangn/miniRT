/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:55:40 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/23 16:09:08 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* Core of the raytracing, get_ray_direction */
void	trace_all_rays(t_global *global)
{
	int			i;
	t_vector	ray_dir;

	i = -1;
	while (++i < global->total_pixels)
	{
		ray_dir.x = global->points[i].point_x;
		ray_dir.y = global->points[i].point_y;
		ray_dir.z = global->points[i].point_z;
		global->c_ray.origin = global->scene.cam.pos;
		global->c_ray.dir = ray_dir;
		global->isecs[i] = find_closest_isec(global);
	}
}

/**
 * Calculates ray direction based on pixel coordinates and camera settings
 * Uses screen dimensions and camera orientation to determine ray vector
 */
t_vector	get_ray_direction(t_global *global, int px_x, int px_y)
{
	static float	aspect_ratio;
	static float	scrn_w;
	static float	scrn_h;
	static float	last_fov;
	t_vector		direction;

	if (last_fov != global->scene.cam.fov)
	{
		aspect_ratio = (float)(WIN_W - MARGIN) / (float)(WIN_H - MARGIN);
		scrn_w = 2.0 * DSCR * tan((global->scene.cam.fov * PI / 180.0) / 2.0);
		scrn_h = scrn_w / aspect_ratio;
		last_fov = global->scene.cam.fov;
	}
	direction.x = (2 * ((px_x + 0.5) / (WIN_W - MARGIN)) - 1) * scrn_w / 2;
	direction.y = (2 * ((px_y + 0.5) / (WIN_H - MARGIN)) - 1) * scrn_h / 2;
	direction = add(add(multiply(global->scene.cam.right_axis, direction.x),
						multiply(global->scene.cam.up_axis, direction.y)),
					global->scene.cam.forward_axis);
	return (norm(direction));
}
