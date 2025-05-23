/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:29:26 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/23 16:38:59 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Creates a new window and initializes the image buffer
 * Sets up the MLX window with the specified dimensions and title
 * Also allocates memory for the image that will be rendered
 * 
 * @param global Structure containing MLX connection and image data
 * @return Pointer to the created window
 */
void	*new_window(t_global *global)
{
	void	*win;

	printf("Creating window\n");
	win = mlx_new_window(global->vars.mlx_conn, WIN_W, WIN_H, "miniRT");
	if (!win)
		finish(global, ERR_WIN);
	global->img.img = mlx_new_image(global->vars.mlx_conn, WIN_W - MARGIN, WIN_H
			- MARGIN);
	if (!global->img.img)
		finish(global, ERR_IMG);
	global->img.addr = mlx_get_data_addr(global->img.img, &global->img.bpp,
			&global->img.bpl, &global->img.endian);
	if (!global->img.addr)
	{
		mlx_destroy_image(global->vars.mlx_conn, global->img.img);
		mlx_destroy_window(global->vars.mlx_conn, win);
		finish(global, ERR_IMG);
	}
	printf("Window created\n");
	return (win);
}
