/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_refractor.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:00:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/21 14:36:05 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Calculates direction of refracted ray using Snell's Law
 */
t_vector	refract_ray(t_vector incident, t_vector normal, float n1, float n2)
{
	float	n_ratio;
	float	cos_i;
	float	cos_t_sq;
	float	cos_t;

	n_ratio = n1 / n2;
	cos_i = -dot(normal, incident);
	cos_t_sq = 1.0f - n_ratio * n_ratio * (1.0f - cos_i * cos_i);
	if (cos_t_sq < 0.0f)
		return (reflect_ray(incident, normal));
	cos_t = sqrt(cos_t_sq);
	return (norm(add(multiply(incident, n_ratio),
				multiply(normal, n_ratio * cos_i - cos_t))));
}

/**
 * Approximates Fresnel effect using Schlick's approximation
 * Returns reflectance (0-1)
 */
float	schlick(float cos_angle, float n1, float n2)
{
	float	r0;
	float	x;

	r0 = ((n1 - n2) / (n1 + n2));
	r0 = r0 * r0;
	x = 1.0f - cos_angle;
	return (r0 + (1.0f - r0) * x * x * x * x * x);
}
