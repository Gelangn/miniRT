/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/13 10:44:07 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* This function needs to exist, but is useless by the moment */
int	handle_no_event(void)
{
	return (SUCCESS);
}
int	mouse_press_hook(int button, int x, int y, t_global *global)
{
	if (button == 1) // Botón izquierdo
	{
		global->mouse_pressed = 1;
		global->last_mouse_x = x;
		global->last_mouse_y = y;
	}
	return (0);
}

int	mouse_release_hook(int button, int x, int y, t_global *global)
{
	(void)x; // Evita warning de parámetro no utilizado
	(void)y; // Evita warning de parámetro no utilizado
	if (button == 1)
		global->mouse_pressed = 0;
	return (0);
}

int	handle_mouse_move(int x, int y, t_global *global)
{
	float		rotation_speed;
	int			dx;
	int			dy;
	t_vector	right;

	if (global->mouse_pressed)
	{
		rotation_speed = 0.005f;
		dx = x - global->last_mouse_x;
		dy = y - global->last_mouse_y;
		// Rotación horizontal (eje Y)
		rotate_camera(global, (t_vector){0, 1, 0}, -dx * rotation_speed);
		// Rotación vertical (eje X)
		right = normalize(cross((t_vector){0, -1, 0},
					normalize(global->scene.cam.orientation)));
		rotate_camera(global, right, -dy * rotation_speed);
		global->last_mouse_x = x;
		global->last_mouse_y = y;
		// Re-renderizar
		render(global);
		mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
			global->img.img, MARGIN / 2, MARGIN / 2);
	}
	return (0);
}

int	handle_mouse_scroll(int button, int x, int y, t_global *global)
{
	(void)global;    // Marca el parámetro como no utilizado
	if (button == 4) // Scroll up
	{
		printf("Mouse scroll up at position (%d, %d)\n", x, y);
		// Añade aquí el código para manejar el desplazamiento hacia arriba
	}
	else if (button == 5) // Scroll down
	{
		printf("Mouse scroll down at position (%d, %d)\n", x, y);
		// Añade aquí el código para manejar el desplazamiento hacia abajo
	}
	return (SUCCESS);
}
