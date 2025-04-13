/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/13 19:27:57 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"
#include <sys/stat.h> // Para mkdir

int	handle_keypress(int keysym, t_global *global)
{
	float		move_speed;
	float		rotate_speed;
	int			needs_render;
	t_vector	z;
	t_vector	x;
	t_vector	y;
	char		filename[100];
	static int	screenshot_count = 1;

	move_speed = 0.5f;
	rotate_speed = 0.05f;
	needs_render = 0;
	// Vectores de dirección
	z = normalize(global->scene.cam.orientation);
	x = normalize(cross((t_vector){0, -1, 0}, z));
	y = cross(z, x);
	// Teclas de movimiento
	if (keysym == XK_Up || keysym == 119) // W o flecha arriba
	{
		move_camera(global, z, move_speed);
		needs_render = 1;
	}
	else if (keysym == XK_Down || keysym == 115) // S o flecha abajo
	{
		move_camera(global, z, -move_speed);
		needs_render = 1;
	}
	else if (keysym == XK_Left || keysym == 97) // A o flecha izquierda
	{
		move_camera(global, x, -move_speed);
		needs_render = 1;
	}
	else if (keysym == XK_Right || keysym == 100) // D o flecha derecha
	{
		move_camera(global, x, move_speed);
		needs_render = 1;
	}
	// Teclas para subir/bajar
	else if (keysym == XK_space) // Espacio
	{
		move_camera(global, y, move_speed);
		needs_render = 1;
	}
	else if (keysym == XK_c) // C
	{
		move_camera(global, y, -move_speed);
		needs_render = 1;
	}
	// Rotación
	else if (keysym == XK_q) // Q
	{
		rotate_camera(global, (t_vector){0, 1, 0}, rotate_speed);
		needs_render = 1;
	}
	else if (keysym == XK_e) // E
	{
		rotate_camera(global, (t_vector){0, 1, 0}, -rotate_speed);
		needs_render = 1;
	}
	else if (keysym == XK_p || keysym == 112) // Tecla 'P'
	{
		// Generar nombre de archivo con contador
		sprintf(filename, "./imgs/screenshot_%03d.bmp", screenshot_count++);
		printf("Guardando imagen en: %s\n", filename);
		// Verificar que el directorio existe
		mkdir("./imgs", 0777); // Crear directorio si no existe
		// Llamada simplificada
		save_bmp(global, filename);
		// No necesitamos re-renderizar, solo mostrar un mensaje
		mlx_string_put(global->vars.mlx_conn, global->vars.mlx_win, WIN_W
			* 0.35, WIN_H * 0.5, 0xFFFFFF, "¡Imagen guardada!");
	}
	else if (keysym == XK_Escape)
		finish(global, SUCCESS);
	// Re-renderizar si hubo cambios
	if (needs_render)
	{
		render(global);
		mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
			global->img.img, MARGIN / 2, MARGIN / 2);
	}
	return (SUCCESS);
}

int	window_close_handler(t_global *global)
{
	finish(global, SUCCESS);
	return (SUCCESS);
}

void	set_hooks(t_global *global)
{
	// Inicializar variables
	global->mouse_pressed = 0;
	global->last_mouse_x = 0;
	global->last_mouse_y = 0;
	// Registro de eventos
	mlx_hook(global->vars.mlx_win, 2, 1L << 0, handle_keypress, global);
	mlx_hook(global->vars.mlx_win, 4, 1L << 2, mouse_press_hook, global);
	mlx_hook(global->vars.mlx_win, 5, 1L << 3, mouse_release_hook, global);
	mlx_hook(global->vars.mlx_win, 6, 1L << 6, handle_mouse_move, global);
	mlx_hook(global->vars.mlx_win, 17, 1L << 17, window_close_handler, global);
}
