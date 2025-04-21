/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_operations.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:15:56 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/21 21:17:32 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Function to calculate the magnitude of a vector sqrt(x^2 + y^2 + z^2)
float	mag(t_vector a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z));
}

// Function to normalize a vector
t_vector	norm(t_vector a)
{
	float		magnitude;
	t_vector	result;

	magnitude = mag(a);
	result.x = a.x / magnitude;
	result.y = a.y / magnitude;
	result.z = a.z / magnitude;
	return (result);
}

// Function to calculate the dot product (producto escalar) of two vectors
float	dot(t_vector a, t_vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

// Function to calculate the cross product of two vectors (producto vectorial)
t_vector	cross(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	return (result);
}

// Function to reflect a vector
t_vector	reflect(t_vector a, t_vector b)
{
	t_vector	result;
	float		dot_product;

	dot_product = dot(a, b);
	result = subtract(a, multiply(b, 2 * dot_product));
	return (result);
}
