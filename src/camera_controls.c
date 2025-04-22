/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:30:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/22 21:49:33 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Move the camera in a specific direction
void	move_camera(t_global *global, t_vector direction, float distance)
{
	t_vector	movement;

	movement = multiply(direction, distance);
	global->scene.cam.pos = add(global->scene.cam.pos, movement);
}

// Rotate the camera around an axis
void	rotate_camera(t_global *global, t_vector axis, float angle)
{
	t_vector	dir;
	t_vector	result;
	float		cos_theta;
	float		sin_theta;
	float		dot_prod;

	if (comp_floats(mag(axis), 0))
		return ;
	dir = global->scene.cam.dir;
	axis = norm(axis);
	cos_theta = cosf(angle);
	sin_theta = sinf(angle);
	dot_prod = dot(axis, dir);
	result.x = dir.x * cos_theta + sin_theta * (axis.y * dir.z - axis.z * dir.y)
		+ (1 - cos_theta) * dot_prod * axis.x;
	result.y = dir.y * cos_theta + sin_theta * (axis.z * dir.x - axis.x * dir.z)
		+ (1 - cos_theta) * dot_prod * axis.y;
	result.z = dir.z * cos_theta + sin_theta * (axis.x * dir.y - axis.y * dir.x)
		+ (1 - cos_theta) * dot_prod * axis.z;
	global->scene.cam.dir = norm(result);
}
