/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:25:38 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/23 16:40:20 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Puts a pixel with the specified color at the given coordinates
 * Performs boundary checks to ensure valid pixel placement
 * 
 * @param img Pointer to the image structure
 * @param px_x X-coordinate of the pixel
 * @param px_y Y-coordinate of the pixel
 * @param color Integer RGB color value to set
 */
void	pixel_put(t_img *img, int px_x, int px_y, int color)
{
	char	*dst;

	if (!img || !img->addr || px_x < 0 || px_x >= WIN_W || px_y < 0
		|| px_y >= WIN_H)
		return ;
	dst = img->addr + (px_y * img->bpl + px_x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}

/**
 * Renders a single pixel based on ray tracing results
 * Calculates lighting and colors for intersections
 * Uses background color for rays that don't hit any object
 * 
 * @param global Structure containing scene and ray data
 * @param index Index of the pixel in the points array
 */
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
		global->c_ray.dir.x = global->points[index].point_x;
		global->c_ray.dir.y = global->points[index].point_y;
		global->c_ray.dir.z = global->points[index].point_z;
		global->c_ray.hit = isec;
		lit_color = cal_lighting(global);
		color = rgb_to_int(lit_color);
	}
	else
		color = DARK_GREY;
	pixel_put(&global->img, x, y, color);
}

/**
 * Renders all pixels in the scene
 * Outputs information about the central ray for debugging
 * Processes each pixel in the scene based on precalculated ray data
 * 
 * @param global Structure containing scene and ray information
 */
void	render_all_pixels(t_global *global)
{
	int	i;
	int	center_idx;

	center_idx = (WIN_H / 2) * (WIN_W - MARGIN) + (WIN_W / 2);
	if (center_idx < global->total_pixels)
	{
		printf("Central ray: type=%d, index=%d, distance=%f\n\n",
			global->isecs[center_idx].obj_type,
			global->isecs[center_idx].obj_index,
			global->isecs[center_idx].dist);
	}
	i = -1;
	while (++i < global->total_pixels)
		render_single_pixel(global, i);
}

/**
 * Prints scene information to the console
 * Outputs camera position, orientation, and scene statistics
 * Useful for debugging and monitoring scene changes
 * 
 * @param global Structure containing scene data
 */
void	print_info(t_global *global)
{
	printf("Camera position: (%.2f, %.2f, %.2f)\n", global->scene.cam.pos.x,
		global->scene.cam.pos.y, global->scene.cam.pos.z);
	printf("Camera orientation: (%.2f, %.2f, %.2f)\n", global->scene.cam.dir.x,
		global->scene.cam.dir.y, global->scene.cam.dir.z);
	printf("Camera up axis: (%.2f, %.2f, %.2f)\n", global->scene.cam.up_axis.x,
		global->scene.cam.up_axis.y, global->scene.cam.up_axis.z);
	printf("Camera right axis: (%.2f, %.2f, %.2f)\n",
		global->scene.cam.right_axis.x,
		global->scene.cam.right_axis.y,
		global->scene.cam.right_axis.z);
	printf("Field of view: %.1f°\n", (float)global->scene.cam.fov);
	printf("Total calculated rays: %d\n", global->total_pixels);
	printf("Scene objects: %d spheres, %d planes, %d cylinders\n",
		global->scene.num_sp, global->scene.num_pl, global->scene.num_cy);
}

/**
 * Main rendering function that coordinates the entire rendering process
 * Handles ray precalculation, intersection testing, and pixel rendering
 * Manages memory allocation and cleanup for ray data
 * 
 * @param global Structure containing all scene and rendering data
 */
void	render(t_global *global)
{
	if (comp_floats(mag(global->scene.cam.dir), 0))
		global->scene.cam.dir = (t_vector){0, 0, 1};
	precal_rays(global);
	global->isecs = malloc(global->total_pixels * sizeof(t_intersec));
	if (!global->isecs)
		finish(global, ERR_MEM);
	if (!global->img.img || !global->img.addr)
	{
		free(global->isecs);
		global->isecs = NULL;
		finish(global, ERR_IMG);
	}
	trace_all_rays(global);
	render_all_pixels(global);
	print_info(global);
	free(global->isecs);
	global->isecs = NULL;
}
