/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_reflection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-mada <bde-mada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:36:23 by bde-mada          #+#    #+#             */
/*   Updated: 2025/06/18 18:40:15 by bde-mada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/**
 * Calculates reflection vector using the formula: R = I - 2(I·N)N
 * where I is incident ray, N is surface normal
 */
t_vector	reflect_ray(t_vector incident, t_vector normal)
{
    float		dot_product;
    t_vector	reflection;

    dot_product = dot(incident, normal);
    reflection = subtract(incident, multiply(normal, 2.0f * dot_product));
    return (norm(reflection));
}

/**
 * Calculates refraction vector using Snell's law
 * Returns zero vector if total internal reflection occurs
 */
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
    
    if (sin_t2 > 1.0f)  // Total internal reflection
        return ((t_vector){0, 0, 0});
    
    cos_t = sqrt(1.0f - sin_t2);
    refracted = add(multiply(incident, eta), 
                    multiply(normal, eta * cos_i - cos_t));
    return (norm(refracted));
}

/**
 * Calculates Fresnel reflectance coefficient
 * Determines how much light is reflected vs refracted
 */
float	fresnel_reflectance(t_vector incident, t_vector normal, float n1, float n2)
{
    float	cos_i;
    float	sin_t2;
    float	cos_t;
    float	r_s;
    float	r_p;

    cos_i = fabs(dot(incident, normal));
    sin_t2 = (n1 / n2) * (n1 / n2) * (1.0f - cos_i * cos_i);
    
    if (sin_t2 > 1.0f)  // Total internal reflection
        return (1.0f);
    
    cos_t = sqrt(1.0f - sin_t2);
    r_s = (n1 * cos_i - n2 * cos_t) / (n1 * cos_i + n2 * cos_t);
    r_p = (n2 * cos_i - n1 * cos_t) / (n2 * cos_i + n1 * cos_t);
    
    return ((r_s * r_s + r_p * r_p) / 2.0f);
}