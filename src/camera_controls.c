/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_controls.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:30:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/22 19:23:36 by anavas-g         ###   ########.fr       */
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
	t_vector	z;
	t_vector	x;
	t_vector	y;
	float		c;
	float		s;

	z = norm(global->scene.cam.dir);
	x = norm(cross(z, (t_vector){0, -1, 0}));
	y = norm(cross(x, z));
	if (comp_floats(mag(axis), 0))
		return ;
	axis = norm(axis);
	c = cosf(angle);
	s = sinf(angle);
	if (comp_floats(axis.x, 1))
		global->scene.cam.dir = add(multiply(z, c), multiply(y, s));
	else if (comp_floats(axis.y, 1))
		global->scene.cam.dir = add(multiply(z, c), multiply(x, s));
	global->scene.cam.dir = norm(global->scene.cam.dir);
}
