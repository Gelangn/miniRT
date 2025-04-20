/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:25:38 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 21:37:50 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	render_single_pixel(t_global *global, int index)
{
	int			x;
	int			y;
	t_intersec	isec;
	int			color;
	t_color		lit_color;

	x = global->points[index].scrn_x;
	y = global->points[index].scrn_y;
	isec = global->isecs[index];
	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	if (isec.obj_type >= 0 && isec.obj_index >= 0)
	{
		global->current_ray_dir.x = global->points[index].point_x;
		global->current_ray_dir.y = global->points[index].point_y;
		global->current_ray_dir.z = global->points[index].point_z;
		global->current_intersec = isec;
		lit_color = cal_lighting(global);
		color = rgb_to_int(lit_color);
	}
	else
		color = DARK_GREY;
	pixel_put(&global->img, x, y, color);
}

void	render_all_pixels(t_global *global)
{
	int	i;
	int	total_pixels;
	int	center_idx;

	total_pixels = (WIN_W - MARGIN) * (WIN_H - MARGIN);
	center_idx = (WIN_H / 2) * (WIN_W - MARGIN) + (WIN_W / 2);
	if (center_idx < total_pixels)
	{
		printf("Central ray: type=%d, index=%d, distance=%f\n",
				global->isecs[center_idx].obj_type,
				global->isecs[center_idx].obj_index,
				global->isecs[center_idx].dist);
	}
	i = -1;
	while (++i < total_pixels)
		render_single_pixel(global, i);
}

void	render(t_global *global)
{
	if (comp_floats(magnitude(global->scene.cam.orientation), 0))
		global->scene.cam.orientation = (t_vector){0, 0, 1};
	precalculate_rays(global);
	global->isecs = malloc((WIN_W - MARGIN) * (WIN_H - MARGIN)
			* sizeof(t_intersec));
	if (!global->isecs)
		finish(global, ERR_MEM);
	if (!global->img.img || !global->img.addr)
	{
		free(global->isecs);
		global->isecs = NULL;
		finish(global, ERR_IMG);
	}
	printf("Camera position: (%f, %f, %f)\n", global->scene.cam.pos.x,
			global->scene.cam.pos.y, global->scene.cam.pos.z);
	printf("Camera orientation: (%f, %f, %f)\n",
			global->scene.cam.orientation.x,
			global->scene.cam.orientation.y,
			global->scene.cam.orientation.z);
	trace_all_rays(global);
	render_all_pixels(global);
	free(global->isecs);
	global->isecs = NULL;
}

void	precalculate_camera_axis(t_global *global)
{
	t_camera	*cam;

	cam = &global->scene.cam;
	cam->z = normalize(cam->orientation);
	cam->x = normalize(cross((t_vector){0, 1, 0}, cam->z));
	cam->y = normalize(cross(cam->z, cam->x));
}
