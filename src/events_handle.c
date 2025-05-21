/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/21 19:56:33 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	handle_movement_keys(t_global *global, int keysym, int *needs_render)
{
	float		move_speed;
	t_vector	forward;
	t_vector	right;
	t_vector	up;

	move_speed = 0.5f;
	forward = norm(global->scene.cam.dir);
	right = norm(cross(forward, (t_vector){0, 1, 0}));
	up = norm(cross(right, forward));
	if (keysym == XK_space)
		move_camera(global, forward, move_speed);
	else if (keysym == XK_b)
		move_camera(global, forward, -move_speed);
	else if (keysym == XK_Left)
		move_camera(global, right, -move_speed);
	else if (keysym == XK_Right)
		move_camera(global, right, move_speed);
	else if (keysym == XK_Up)
		move_camera(global, up, move_speed);
	else if (keysym == XK_Down)
		move_camera(global, up, -move_speed);
	else
		return ;
	*needs_render = 1;
}

/* Main rotation handling function */
void	handle_rotation_keys(t_global *global, int keysym, int *needs_render)
{
	float	rotate_speed;

	rotate_speed = 0.05f;
	if (keysym == 119 || keysym == 115 || keysym == 97 || keysym == 100)
		apply_standard_rotation(global, keysym, rotate_speed);
	else if (keysym == 113 || keysym == 101)
		apply_roll_rotation(global, keysym, rotate_speed);
	else
		return ;
	*needs_render = 1;
}

void	handle_zoom_keys(t_global *global, int keysym)
{
	if (keysym == 65451 || keysym == 61 || keysym == 43)
	{
		if (global->scene.cam.fov > 10.0f)
		{
			global->scene.cam.fov -= 5.0f;
			if (global->scene.cam.fov < 10.0f)
				global->scene.cam.fov = 10.0f;
			update_display(global);
		}
	}
	else if (keysym == 65453 || keysym == 45)
	{
		if (global->scene.cam.fov < 120.0f)
		{
			global->scene.cam.fov += 5.0f;
			if (global->scene.cam.fov > 120.0f)
				global->scene.cam.fov = 120.0f;
			update_display(global);
		}
	}
}

int	handle_keypress(int keysym, t_global *global)
{
	int	needs_render;

	printf("Key pressed: %d\n", keysym);
	needs_render = 0;
	if (keysym == XK_Escape)
		finish(global, SUCCESS);
	handle_movement_keys(global, keysym, &needs_render);
	handle_rotation_keys(global, keysym, &needs_render);
	handle_zoom_keys(global, keysym);
	if (keysym == XK_p || keysym == 112)
		handle_screenshot(global);
	if (needs_render)
		update_display(global);
	return (SUCCESS);
}
