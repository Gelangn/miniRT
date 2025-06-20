/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   window.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:29:26 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/20 17:37:29 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"
#include <float.h>

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
	
	printf("\nCreating window\n");
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
			#ifdef DEBUG_TRANSPARENCY
			// Debug transparency objects using scene counters
			int i;
			int valid_spheres;
			{
				printf("\n=== TRANSPARENCY DEBUG ===\n");
				valid_spheres = 0;
				i = 0;
				while (i < global->scene.num_sp && global->scene.spheres)
				{
					printf("Sphere %d: pos(%.1f,%.1f,%.1f) transp=%.2f\n",
						i,
						global->scene.spheres[i].center.x,
						global->scene.spheres[i].center.y,
						global->scene.spheres[i].center.z,
						global->scene.spheres[i].transp);
						if (global->scene.spheres[i].transp > 0.001)
						valid_spheres++;
						i++;
					}
					printf("Transparent spheres found: %d out of %d total\n", valid_spheres,
						global->scene.num_sp);
						i = 0;
						while (i < global->scene.num_pl && global->scene.planes)
						{
							if (global->scene.planes[i].transp > 0.001)
							{
								printf("Transparent plane %d: transp=%.2f\n",
									i,
									global->scene.planes[i].transp);
								}
								i++;
							}
							printf("=========================\n\n");
						}
						#endif
						return (win);
					}
					