/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/22 22:19:54 by anavas-g         ###   ########.fr       */
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
	if (keysym == XK_Up)
		move_camera(global, forward, move_speed);
	else if (keysym == XK_Down)
		move_camera(global, forward, -move_speed);
	else if (keysym == XK_Left)
		move_camera(global, right, -move_speed);
	else if (keysym == XK_Right)
		move_camera(global, right, move_speed);
	else if (keysym == XK_space)
		move_camera(global, up, move_speed);
	else if (keysym == XK_c)
		move_camera(global, up, -move_speed);
	else
		return ;
	*needs_render = 1;
}

// Función auxiliar para la rotación estándar (W, A, S, D)
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

// Función auxiliar para la rotación roll (Q, E)
void	apply_roll_rotation(t_global *global, int key, float speed)
{
	float	angle;

	if (key == 113)
		angle = -speed;
	else
		angle = speed;
	precal_camera_axis(global);
	global->scene.cam.right_axis = norm(add(
				multiply(global->scene.cam.right_axis, cosf(angle)),
				multiply(global->scene.cam.up_axis, sinf(angle))));
	global->scene.cam.up_axis = norm(cross(global->scene.cam.forward_axis,
				global->scene.cam.right_axis));
	rotate_camera(global, global->scene.cam.forward_axis, angle);
}

// Función principal de manejo de rotación simplificada
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
	if (keysym == 65451 || keysym == 61)
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

void	handle_screenshot(t_global *global)
{
	static int	screenshot_count;
	char		filename[100];

	screenshot_count = 1;
	mkdir("./imgs", 0777);
	sprintf(filename, "./imgs/screenshot_%03d.bmp", screenshot_count++);
	printf("Guardando imagen en: %s\n", filename);
	save_bmp(global, filename);
	mlx_string_put(global->vars.mlx_conn, global->vars.mlx_win, WIN_W * 0.35,
			WIN_H * 0.5, 0xFFFFFF, "Imagen guardada!");
}

int	handle_keypress(int keysym, t_global *global)
{
	int	needs_render;

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
