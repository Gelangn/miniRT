/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 00:38:31 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"
#include <sys/stat.h> // Para mkdir

// Funciones auxiliares para manejar diferentes tipos de eventos
void	handle_movement_keys(t_global *global, int keysym, int *needs_render)
{
	float		move_speed;
	t_vector	z;
	t_vector	x;
	t_vector	y;

	move_speed = 0.5f;
	z = normalize(global->scene.cam.orientation);
	x = normalize(cross((t_vector){0, -1, 0}, z));
	y = cross(z, x);
	// Movimientos básicos de cámara
	if (keysym == XK_Up || keysym == 119) // W o flecha arriba
		move_camera(global, z, move_speed);
	else if (keysym == XK_Down || keysym == 115) // S o flecha abajo
		move_camera(global, z, -move_speed);
	else if (keysym == XK_Left || keysym == 97) // A o flecha izquierda
		move_camera(global, x, -move_speed);
	else if (keysym == XK_Right || keysym == 100) // D o flecha derecha
		move_camera(global, x, move_speed);
	else if (keysym == XK_space) // Espacio (subir)
		move_camera(global, y, move_speed);
	else if (keysym == XK_c) // C (bajar)
		move_camera(global, y, -move_speed);
	else
		return ; // No se detectó tecla de movimiento
	*needs_render = 1;
}

void	handle_rotation_keys(t_global *global, int keysym, int *needs_render)
{
	float	rotate_speed;

	rotate_speed = 0.05f;
	if (keysym == XK_q) // Rotación Q
		rotate_camera(global, (t_vector){0, 1, 0}, rotate_speed);
	else if (keysym == XK_e) // Rotación E
		rotate_camera(global, (t_vector){0, 1, 0}, -rotate_speed);
	else
		return ; // No se detectó tecla de rotación
	*needs_render = 1;
}

void	handle_zoom_keys(t_global *global, int keysym)
{
	// Zoom in con la tecla '+'
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
	// Zoom out con la tecla '-'
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
	static int	screenshot_count = 1;
	char		filename[100];

	// Crear directorio si no existe
	mkdir("./imgs", 0777);
	// Generar nombre de archivo con contador
	sprintf(filename, "./imgs/screenshot_%03d.bmp", screenshot_count++);
	printf("Guardando imagen en: %s\n", filename);
	// Guardar imagen
	save_bmp(global, filename);
	// Mostrar mensaje de confirmación
	mlx_string_put(global->vars.mlx_conn, global->vars.mlx_win, WIN_W * 0.35,
		WIN_H * 0.5, 0xFFFFFF, "¡Imagen guardada!");
}

// Función centralizada para actualizar la pantalla después de cambios
void	update_display(t_global *global)
{
	render(global);
	mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
		global->img.img, MARGIN / 2, MARGIN / 2);
}

// Función principal simplificada
int	handle_keypress(int keysym, t_global *global)
{
	int	needs_render;

	needs_render = 0;
	// Salir con Escape
	if (keysym == XK_Escape)
		finish(global, SUCCESS);
	// Agrupar manejo de teclas por categorías
	handle_movement_keys(global, keysym, &needs_render);
	handle_rotation_keys(global, keysym, &needs_render);
	// Manejar zoom (estas funciones controlan internamente su renderizado)
	handle_zoom_keys(global, keysym);
	// Captura de pantalla
	if (keysym == XK_p || keysym == 112)
		handle_screenshot(global);
	// Renderizar si es necesario
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
	// Eventos de teclado
	mlx_hook(global->vars.mlx_win, 2, 1L << 0, handle_keypress, global);
	// Evento de cierre de ventana
	mlx_hook(global->vars.mlx_win, 17, 1L << 17, window_close_handler, global);
	// Eventos de ratón
	mlx_hook(global->vars.mlx_win, 4, 1L << 2, mouse_press_hook, global);
	mlx_hook(global->vars.mlx_win, 5, 1L << 3, mouse_release_hook, global);
	mlx_hook(global->vars.mlx_win, 6, 1L << 6, handle_mouse_move, global);
	mlx_mouse_hook(global->vars.mlx_win, handle_mouse_scroll, global);
}
