/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_render.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:20:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/05 20:18:12 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

static void	cal_ray_for_pixel(t_global *global, int px_x, int px_y, int idx)
{
	t_vector	dir;

	global->points[idx].scrn_x = px_x;
	global->points[idx].scrn_y = px_y;
	dir = get_ray_direction(global, px_x, px_y);
	global->points[idx].point_x = dir.x;
	global->points[idx].point_y = dir.y;
	global->points[idx].point_z = dir.z;
}

void	precal_camera_axis(t_global *global)
{
	t_camera	*cam;
	float		cos_angle;
	float		sin_angle;

	cam = &global->scene.cam;
	cam->forward_axis = norm(cam->dir);
	cam->right_axis = norm(cross((t_vector){0, 1, 0}, cam->forward_axis));
	cam->up_axis = norm(cross(cam->forward_axis, cam->right_axis));
	if (cam->roll_angle != 0)
	{
		cos_angle = cosf(cam->roll_angle);
		sin_angle = sinf(cam->roll_angle);
		cam->right_axis = norm(add(multiply(cam->right_axis, cos_angle),
					multiply(cam->up_axis, sin_angle)));
		cam->up_axis = norm(cross(cam->forward_axis, cam->right_axis));
	}
}

void	precal_rays(t_global *global)
{
	int	px_x;
	int	px_y;
	int	i;

	if (global->points)
		free(global->points);
	global->points = malloc((WIN_W - MARGIN) * (WIN_H - MARGIN)
			* sizeof(t_point));
	if (!global->points)
		finish(global, ERR_MEM);
	precal_camera_axis(global);
	i = 0;
	px_x = -1;
	while (++px_x < (WIN_W - MARGIN))
	{
		px_y = -1;
		while (++px_y < (WIN_H - MARGIN))
			cal_ray_for_pixel(global, px_x, px_y, i++);
	}
}
