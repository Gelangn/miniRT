/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_core.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:55:40 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 21:37:35 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* Core of the raytracing, calls cal_ray and in turn get_ray_direction */
void	trace_all_rays(t_global *global)
{
	int			i;
	int			total_pixels;
	t_vector	ray_dir;

	total_pixels = (WIN_W - MARGIN) * (WIN_H - MARGIN);
	i = -1;
	while (++i < total_pixels)
	{
		ray_dir.x = global->points[i].point_x;
		ray_dir.y = global->points[i].point_y;
		ray_dir.z = global->points[i].point_z;
		global->current_ray_origin = global->scene.cam.pos;
		global->current_ray_dir = ray_dir;
		global->isecs[i] = find_closest_intersec(global);
	}
	printf("Total calculated rays: %d\n", total_pixels);
}

t_intersec	cal_ray(t_global *global, int px_x, int px_y)
{
	global->current_ray_origin = global->scene.cam.pos;
	global->current_ray_dir = get_ray_direction(global, px_x, px_y);
	return (find_closest_intersec(global));
}

/* Here we define the environment, axis directions,
	universe center (camera/observer),
   and screen relationships to take advantage of the entire view */
t_vector	get_ray_direction(t_global *global, int px_x, int px_y)
{
	t_camera	cam;

	static float aspect_ratio, scrn_w, scrn_h, last_fov = -1;
	float u, v;
	cam = global->scene.cam;
	if (last_fov != cam.fov)
	{
		aspect_ratio = (float)(WIN_W - MARGIN) / (float)(WIN_H - MARGIN);
		scrn_w = 2.0 * DSCR * tan((cam.fov * PI / 180.0) / 2.0);
		scrn_h = scrn_w / aspect_ratio;
		last_fov = cam.fov;
	}
	u = (2 * ((px_x + 0.5) / (WIN_W - MARGIN)) - 1) * scrn_w / 2;
	v = (2 * ((px_y + 0.5) / (WIN_H - MARGIN)) - 1) * scrn_h / 2;
	return (normalize(add(add(multiply(cam.x, u),
								multiply(cam.y, v)),
							cam.z)));
}
