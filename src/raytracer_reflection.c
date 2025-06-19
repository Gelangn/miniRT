/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_reflection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 14:30:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 14:32:53 by anavas-g         ###   ########.fr       */
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
	float		n_ratio;
	float		cos_i;
	float		sin_t2;
	float		cos_t;
	t_vector	refracted;

	n_ratio = n1 / n2;
	cos_i = -dot(normal, incident);
	// Check for total internal reflection
	sin_t2 = n_ratio * n_ratio * (1.0f - cos_i * cos_i);
	if (sin_t2 > 1.0f)
		return ((t_vector){0, 0, 0}); // Total internal reflection
	cos_t = sqrtf(1.0f - sin_t2);
	refracted = add(multiply(incident, n_ratio),
					multiply(normal, n_ratio * cos_i - cos_t));
	return (norm(refracted));
}

float	fresnel_reflectance(t_vector incident, t_vector normal, float n1,
		float n2)
{
	float cos_i;
	float sin_t;
	float cos_t;
	float rs;
	float rp;

	cos_i = fabsf(dot(normal, incident));
	sin_t = (n1 / n2) * sqrtf(fmaxf(0.0f, 1.0f - cos_i * cos_i));

	if (sin_t >= 1.0f)
		return (1.0f); // Total internal reflection

	cos_t = sqrtf(fmaxf(0.0f, 1.0f - sin_t * sin_t));

	rs = ((n1 * cos_i - n2 * cos_t) / (n1 * cos_i + n2 * cos_t));
	rp = ((n1 * cos_t - n2 * cos_i) / (n1 * cos_t + n2 * cos_i));

	return ((rs * rs + rp * rp) / 2.0f);
}