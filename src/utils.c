/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:27:57 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/05 20:01:20 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Function to compare two floats using EPSILON
int	comp_floats(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

// Returns true if a is less than b-EPSILON
int	is_less_than(float a, float b)
{
	return (a < b - EPSILON);
}

// Returns true if a is greater than b+EPSILON
int	is_greater_than(float a, float b)
{
	return (a > b + EPSILON);
}

int	is_valid_pixel(int px_x, int px_y, int width, int height)
{
	return (px_x >= 0 && px_x < width && px_y >= 0 && px_y < height);
}

int	rgb_to_int(t_color color)
{
	return ((color.r << 16) | (color.g << 8) | color.b);
}
