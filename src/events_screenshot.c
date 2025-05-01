/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_screenshot.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 00:07:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/02 00:08:23 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

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
