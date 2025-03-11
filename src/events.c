/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/03/11 18:30:38 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

int	handle_keypress(int key, t_global *global)
{
	ft_printf("Key %d is pressed!\n", key);
	if (key == 53 || key == 65307 || key == 0)
	{
		close_window(&global->vars);
		ft_printf("Closed window\n");
		exit(0);
	}
	return (0);
}

int	handle_keyrelease(int key)
{
	ft_printf("Keyrelease: %d\n", key);
	return (0);
}

int	handle_mouse_move(int x, int y)
{
	printf("Mouse is at position (%d, %d)\n", x, y);
	return (0);
}
