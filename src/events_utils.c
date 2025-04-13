/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/13 20:25:13 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* This function needs to exist, but is useless by the moment */
int	handle_no_event(void)
{
	return (SUCCESS);
}
int	mouse_press_hook(int button, int pos_x, int pos_y, t_global *global)
{
	if (button == 1) // Botón izquierdo
	{
		global->mouse_pressed = 1;
		global->last_mouse_x = pos_x;
		global->last_mouse_y = pos_y;
	}
	return (0);
}

int	mouse_release_hook(int button, int pos_x, int pos_y, t_global *global)
{
	(void)pos_x; // Evita warning de parámetro no utilizado
	(void)pos_y; // Evita warning de parámetro no utilizado
	if (button == 1)
		global->mouse_pressed = 0;
	return (0);
}

int	handle_mouse_move(int pos_x, int pos_y, t_global *global)
{
	float		rotation_speed;
	int			dx;
	int			dy;
	t_vector	vc_x;

	if (global->mouse_pressed)
	{
		rotation_speed = 0.005f;
		dx = pos_x - global->last_mouse_x;
		dy = pos_y - global->last_mouse_y;
		// Rotación horizontal (eje Y)
		rotate_camera(global, (t_vector){0, 1, 0}, -dx * rotation_speed);
		// Rotación vertical (eje X)
		vc_x = multiply(normalize(cross((t_vector){0, -1, 0},
						normalize(global->scene.cam.orientation))), -1.0f);
		normalize(global->scene.cam.orientation);
		rotate_camera(global, vc_x, -dy * rotation_speed);
		global->last_mouse_x = pos_x;
		global->last_mouse_y = pos_y;
		// Re-renderizar
		render(global);
		mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
			global->img.img, MARGIN / 2, MARGIN / 2);
	}
	return (0);
}

int	handle_mouse_scroll(int button, int pos_x, int pos_y, t_global *global)
{
	float	zoom_factor;

	// Añadir mensaje de depuración
	printf("Evento de mouse: botón %d en posición (%d, %d)\n", button, pos_x,
		pos_y);
	zoom_factor = 5.0f;
	(void)pos_x;
	(void)pos_y;
	// Comprobar todos los posibles botones para scroll
	if (button == 4) // Scroll up - Zoom in (probamos valores alternativos)
	{
		// Reducir el FOV para hacer zoom in
		if (global->scene.cam.fov > 10.0f)
		{
			global->scene.cam.fov -= zoom_factor;
			if (global->scene.cam.fov < 10.0f)
				global->scene.cam.fov = 10.0f;
			// Re-renderizar con el nuevo FOV
			render(global);
			mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
				global->img.img, MARGIN / 2, MARGIN / 2);
		}
	}
	else if (button == 5) // Scroll down - Zoom out
	{
		// Aumentar el FOV para hacer zoom out
		if (global->scene.cam.fov < 120.0f)
		{
			global->scene.cam.fov += zoom_factor;
			if (global->scene.cam.fov > 120.0f)
				global->scene.cam.fov = 120.0f;
			// Re-renderizar con el nuevo FOV
			render(global);
			mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
				global->img.img, MARGIN / 2, MARGIN / 2);
		}
	}
	return (SUCCESS);
}
