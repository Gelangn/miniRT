/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:15:56 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 17:16:02 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Function to sum tuples
t_vector	add(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
	return (result);
}

// Function to subtract tuples
t_vector	subtract(t_vector a, t_vector b)
{
	t_vector	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	result.z = a.z - b.z;
	return (result);
}

// Function to calculate the dot product (producto escalar) of two vectors
float	dot(t_vector a, t_vector b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

// Function to multiply a tuple by a scalar
t_vector	multiply(t_vector a, float scalar)
{
	t_vector	result;

	result.x = a.x * scalar;
	result.y = a.y * scalar;
	result.z = a.z * scalar;
	return (result);
}

// Function to divide a tuple by a scalar
t_vector	divide(t_vector a, float scalar)
{
	t_vector	result;

	result.x = a.x / scalar;
	result.y = a.y / scalar;
	result.z = a.z / scalar;
	return (result);
}

// Function to calculate the magnitude of a vector
float	magnitude(t_vector a)
{
	return (sqrt(a.x * a.x + a.y * a.y + a.z * a.z)); // sqrt(x^2 + y^2 + z^2)
}

// Function to normalize a vector
t_vector	normalize(t_vector a)
{
	float		mag;
	t_vector	result;

	mag = magnitude(a);
	result.x = a.x / mag;
	result.y = a.y / mag;
	result.z = a.z / mag;
	return (result);
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
