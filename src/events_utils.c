/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/07 21:27:02 by anavas-g          #+#    #+#             */
/*   Updated: 2025/03/25 07:46:19 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* This function needs to exist, but is useless by the moment */
int	handle_no_event(void)
{
	return (0);
}

int	mouse_press_hook(int button, int x, int y)
{
	ft_printf("Mouse button %d pressed at position (%d, %d)\n", button, x, y);
	return (0);
}

int	mouse_release_hook(int button, int x, int y)
{
	ft_printf("Mouse button %d released at position (%d, %d)\n", button, x, y);
	return (0);
}
