/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 22:50:21 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	handle_movement_keys(t_global *global, int keysym, int *needs_render)
{
	float		move_speed;
	t_vector	z;
	t_vector	x;
	t_vector	y;

	move_speed = 0.5f;
	z = norm(global->scene.cam.orientation);
	x = norm(cross((t_vector){0, -1, 0}, z));
	y = cross(z, x);
	if (keysym == XK_Up || keysym == 119)
		move_camera(global, z, move_speed);
	else if (keysym == XK_Down || keysym == 115)
		move_camera(global, z, -move_speed);
	else if (keysym == XK_Left || keysym == 97)
		move_camera(global, x, -move_speed);
	else if (keysym == XK_Right || keysym == 100)
		move_camera(global, x, move_speed);
	else if (keysym == XK_space)
		move_camera(global, y, move_speed);
	else if (keysym == XK_c)
		move_camera(global, y, -move_speed);
	else
		return ;
	*needs_render = 1;
}

void	handle_rotation_keys(t_global *global, int keysym, int *needs_render)
{
	float	rotate_speed;

	rotate_speed = 0.05f;
	if (keysym == XK_q)
		rotate_camera(global, (t_vector){0, 1, 0}, rotate_speed);
	else if (keysym == XK_e)
		rotate_camera(global, (t_vector){0, 1, 0}, -rotate_speed);
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


void	update_display(t_global *global)
{
	render(global);
	mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
		global->img.img, MARGIN / 2, MARGIN / 2);
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

int	window_close_handler(t_global *global)
{
	finish(global, SUCCESS);
	return (SUCCESS);
}

void	set_hooks(t_global *global)
{
	mlx_hook(global->vars.mlx_win, 2, 1L << 0, handle_keypress, global);
	mlx_hook(global->vars.mlx_win, 17, 1L << 17, window_close_handler, global);
	mlx_hook(global->vars.mlx_win, 4, 1L << 2, mouse_press_hook, global);
	mlx_hook(global->vars.mlx_win, 5, 1L << 3, mouse_release_hook, global);
	mlx_hook(global->vars.mlx_win, 6, 1L << 6, handle_mouse_move, global);
	mlx_mouse_hook(global->vars.mlx_win, handle_mouse_scroll, global);
}
