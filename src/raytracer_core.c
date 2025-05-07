/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:55:40 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/07 18:57:26 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* Core of the raytracing, calls cal_ray and in turn get_ray_direction */
void	trace_all_rays(t_global *global)
{
	clock_t		start;
	int			i;
	int			total_pixels;
	t_vector	ray_dir;
	clock_t		end_init;
	clock_t		end;

	start = clock();
	// Inicializar BVH antes de calcular rayos
	init_bvh(global);
	end_init = clock();
	total_pixels = (WIN_W - MARGIN) * (WIN_H - MARGIN);
	i = -1;
	while (++i < total_pixels)
	{
		ray_dir.x = global->points[i].point_x;
		ray_dir.y = global->points[i].point_y;
		ray_dir.z = global->points[i].point_z;
		global->c_ray.origin = global->scene.cam.pos;
		global->c_ray.dir = ray_dir;
		global->isecs[i] = find_closest_intersec_bvh(global); // Usar BVH aquí
	}
	printf("Total calculated rays: %d\n", total_pixels);
	end = clock();
	printf("BVH initialization: %.2f ms\n",
			1000.0 * (end_init - start) / CLOCKS_PER_SEC);
	printf("Total raytracing: %.2f ms\n",
			1000.0 * (end - end_init) / CLOCKS_PER_SEC);
}

t_intersec	cal_ray(t_global *global, int px_x, int px_y)
{
	global->c_ray.origin = global->scene.cam.pos;
	global->c_ray.dir = get_ray_direction(global, px_x, px_y);
	return (find_closest_intersec(global));
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

	last_fov = -1;
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
