/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/21 19:48:45 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* This function needs to exist, but is useless by the moment */
int	handle_no_event(void)
{
	return (SUCCESS);
}

void	update_display(t_global *global)
{
	render(global);
	mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
		global->img.img, MARGIN / 2, MARGIN / 2);
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
