/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_render.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:20:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/23 16:43:13 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Calculates and updates the camera coordinate system
 * Sets the forward, right, and up axis vectors based on camera orientation
 * Also handles roll angle adjustments if applicable
 * 
 * @param global Structure containing scene and camera information
 */
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
