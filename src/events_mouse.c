/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_mouse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/05 20:01:51 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	mouse_press_hook(int button, int pos_x, int pos_y, t_global *global)
{
	printf("Mouse press: button=%d, pos=(%d,%d)\n", button, pos_x, pos_y);
	if (button == 4 || button == 5)
		return (handle_mouse_scroll(button, pos_x, pos_y, global));
	if (button == 1)
	{
		global->mouse_pressed = 1;
		global->last_mouse_x = pos_x;
		global->last_mouse_y = pos_y;
		printf("Set mouse_pressed to 1\n");
	}
	return (0);
}

int	mouse_release_hook(int button, int pos_x, int pos_y, t_global *global)
{
	(void)pos_x;
	(void)pos_y;
	printf("Mouse release: button=%d\n", button);
	if (button == 1)
	{
		global->mouse_pressed = 0;
		printf("Set mouse_pressed to 0\n");
	}
	return (0);
}

int	handle_mouse_move(int pos_x, int pos_y, t_global *global)
{
	float		rotation_speed;
	int			dx;
	int			dy;
	t_vector	vc_x;

	printf("Mouse move: pos=(%d,%d), pressed=%d\n", pos_x, pos_y,
		global->mouse_pressed);
	if (global->mouse_pressed)
	{
		rotation_speed = 0.005f;
		dx = pos_x - global->last_mouse_x;
		dy = pos_y - global->last_mouse_y;
		rotate_camera(global, (t_vector){0, 1, 0}, -dx * rotation_speed);
		vc_x = multiply(norm(cross((t_vector){0, -1, 0},
						norm(global->scene.cam.dir))), -1.0f);
		norm(global->scene.cam.dir);
		rotate_camera(global, vc_x, -dy * rotation_speed);
		global->last_mouse_x = pos_x;
		global->last_mouse_y = pos_y;
		render(global);
		mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
			global->img.img, MARGIN / 2, MARGIN / 2);
	}
	return (0);
}

void	adjust_camera_fov(t_global *global, float delta)
{
	float	new_fov;

	new_fov = global->scene.cam.fov + delta;
	if (new_fov < 10.0f)
		new_fov = 10.0f;
	else if (new_fov > 120.0f)
		new_fov = 120.0f;
	if (new_fov != global->scene.cam.fov)
	{
		global->scene.cam.fov = new_fov;
		update_display(global);
	}
}

int	handle_mouse_scroll(int button, int pos_x, int pos_y, t_global *global)
{
	float	zoom_factor;

	(void)pos_x;
	(void)pos_y;
	zoom_factor = 5.0f;
	if (button == 4)
		adjust_camera_fov(global, -zoom_factor);
	else if (button == 5)
		adjust_camera_fov(global, zoom_factor);
	return (SUCCESS);
}
