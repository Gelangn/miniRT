/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/03/25 08:56:44 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	handle_keypress(int key, t_global *global)
{
	if (key == 53 || key == 65307) // 65307 is ESC key in Linux (X11)
	{
		ft_printf("Key %d (ESC) is pressed!\n", key);
		close_window(global);
	}
	else
		ft_printf("Key %d is pressed!\n", key);
	return (0);
}

int	handle_keyrelease(int key)
{
	ft_printf("Key %d is released!\n", key);
	return (0);
}

int	handle_mouse_move(int x, int y)
{
	printf("Mouse is at position (%d, %d)\n", x, y);
	return (0);
}
void	set_hooks(t_global *global)
{
	mlx_do_key_autorepeatoff(global->vars.mlx_conn);
	mlx_loop_hook(global->vars.mlx_conn, &handle_no_event, &global->vars);  
		// No event
	mlx_hook(global->vars.mlx_win, 2, 1L << 0, &handle_keypress, global);   
		// Key press
	mlx_hook(global->vars.mlx_win, 3, 1L << 1, &handle_keyrelease, global); 
		// Key release
	mlx_hook(global->vars.mlx_win, 4, 1L << 2, &mouse_press_hook, global);  
		// Mouse press
	mlx_hook(global->vars.mlx_win, 5, 1L << 3, &mouse_release_hook, global);
		// Mouse release
	mlx_hook(global->vars.mlx_win, 6, 1L << 6, &handle_mouse_move, global); 
		// Mouse move
	mlx_hook(global->vars.mlx_win, 17, 0, &close_window, global);           
		// Close window
}
