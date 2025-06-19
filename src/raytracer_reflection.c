/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_reflection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:30:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 13:55:10 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_vector	reflect_ray(t_vector incident, t_vector normal)
{
	float		dot_product;
	t_vector	reflection;

	dot_product = dot(incident, normal);
	reflection = subtract(incident, multiply(normal, 2.0f * dot_product));
	return (norm(reflection));
}

t_vector	refract_ray(t_vector incident, t_vector normal, float n1, float n2)
{
	float		cos_i;
	float		sin_t2;
	float		cos_t;
	float		eta;
	t_vector	refracted;

	eta = n1 / n2;
	cos_i = -dot(incident, normal);
	sin_t2 = eta * eta * (1.0f - cos_i * cos_i);
	if (sin_t2 > 1.0f)
		return ((t_vector){0, 0, 0});
	cos_t = sqrt(1.0f - sin_t2);
	refracted = add(multiply(incident, eta),
					multiply(normal, eta * cos_i - cos_t));
	return (norm(refracted));
}

float	fresnel_reflectance(t_vector incident, t_vector normal, float n1,
		float n2)
{
	(void)incident;
	(void)normal;
	(void)n1;
	(void)n2;
	return (0.1f);
}