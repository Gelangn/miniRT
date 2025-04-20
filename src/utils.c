/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:27:57 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 18:10:32 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	finish(t_global *global, const char *message)
{
	if (errno == 0)
		ft_putendl_fd((char *)message, 2);
	else
		perror(message);
	if (global)
		free_global(global);
	printf("Exiting program\n");
	exit(SUCCESS);
}

// Function to compare two floats using EPSILON
int	comp_floats(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

// Devuelve true si a es menor que b-EPSILON
int	is_less_than(float a, float b)
{
	return (a < b - EPSILON);
}

// Devuelve true si a es mayor que b+EPSILON
int	is_greater_than(float a, float b)
{
	return (a > b + EPSILON);
}

int	is_valid_pixel(int px_x, int px_y, int width, int height)
{
	return (px_x >= 0 && px_x < width && px_y >= 0 && px_y < height);
}
