/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_rotation.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:06:45 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/02 00:21:03 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/* Helper function for standard rotation (W, A, S, D) */
void	apply_standard_rotation(t_global *global, int key, float speed)
{
	if (key == 119)
		rotate_camera(global, norm(cross((t_vector){0, -1, 0},
					norm(global->scene.cam.dir))), speed);
	else if (key == 115)
		rotate_camera(global, norm(cross((t_vector){0, -1, 0},
					norm(global->scene.cam.dir))), -speed);
	else if (key == 97)
		rotate_camera(global, (t_vector){0, 1, 0}, speed);
	else if (key == 100)
		rotate_camera(global, (t_vector){0, 1, 0}, -speed);
}

/* Helper function for roll rotation (Q, E) */
void	apply_roll_rotation(t_global *global, int key, float speed)
{
	float	angle;

	if (key == 113)
		angle = -speed;
	else
		angle = speed;
	global->scene.cam.roll_angle += angle;
	precal_camera_axis(global);
}
