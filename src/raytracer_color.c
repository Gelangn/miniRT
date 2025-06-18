/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-mada <bde-mada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:06:30 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/18 18:36:23 by bde-mada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Calculates the ambient lighting component
 * Ambient light provides basic illumination to all objects in the scene
 * regardless of their position or orientation
 * 
 * @param global Structure containing scene information and ray context
 * @return The RGB color representing ambient lighting contribution
 */
t_color	cal_ambient(t_global *global)
{
	t_color	result;

	result.r = global->c_ray.obj_color.r * global->scene.ambient.intensity;
	result.g = global->c_ray.obj_color.g * global->scene.ambient.intensity;
	result.b = global->c_ray.obj_color.b * global->scene.ambient.intensity;
	return (result);
}

/**
 * Calculates the diffuse lighting component using Lambert's cosine law
 * Diffuse lighting depends on the angle between the surface normal
 * and the direction to the light source
 * 
 * @param global Structure containing scene information and ray context
 * @return The RGB color representing diffuse lighting contribution
 */
t_color	cal_diffuse(t_global *global)
{
	float	diff;
	float	light_intensity;

	light_intensity = global->scene.light.intensity * 1.2f;
	diff = fmax(0.0f, dot(global->c_ray.normal, global->c_light.dir));
	return (color_scale(global->c_ray.obj_color, light_intensity * diff));
}

/**
 * Calculates the specular highlight component using Phong reflection model
 * Specular highlights represent glossy reflections of light sources
 * and depend on the viewing angle relative to the reflection direction
 * 
 * @param global Structure containing scene information and ray context
 * @return The RGB color representing specular highlight contribution
 */
t_color	cal_specular(t_global *global)
{
	t_vector	view_dir;
	t_vector	reflect_dir;
	float		spec;
	t_color		white;
	float		light_intensity;

	light_intensity = global->scene.light.intensity;
	view_dir = norm(multiply(global->c_ray.dir, -1.0f));
	reflect_dir = subtract(multiply(global->c_ray.normal, 2.0f
				* dot(global->c_ray.normal, global->c_light.dir)),
			global->c_light.dir);
	spec = pow(fmax(0.0f, dot(view_dir, reflect_dir)), 200);
	white.r = 255;
	white.g = 255;
	white.b = 255;
	return (color_scale(white, spec * light_intensity * 0.5f));
}

/**
 * Clamps RGB color values to ensure they remain in the valid range [0-255]
 * Colors outside this range can cause rendering artifacts or overflow issues
 * 
 * @param color Pointer to a color structure to be clamped
 */
void	clamp_color(t_color *color)
{
	color->r = fmin(255, fmax(0, color->r));
	color->g = fmin(255, fmax(0, color->g));
	color->b = fmin(255, fmax(0, color->b));
}
