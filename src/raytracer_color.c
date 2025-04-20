/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_color.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 21:06:30 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 22:18:08 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

t_color	calculate_ambient(t_global *global)
{
	t_color	result;

	result.r = global->c_ray.obj_color.r * global->scene.ambient.intensity;
	result.g = global->c_ray.obj_color.g * global->scene.ambient.intensity;
	result.b = global->c_ray.obj_color.b * global->scene.ambient.intensity;
	return (result);
}

t_color	calculate_diffuse(t_global *global)
{
	float	diff;
	float	light_intensity;

	light_intensity = global->scene.light.intensity;
	diff = fmax(0.0f, dot(global->c_ray.normal, global->c_light.dir));
	return (color_scale(global->c_ray.obj_color, light_intensity * diff));
}

t_color	calculate_specular(t_global *global)
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
	spec = pow(fmax(0.0f, dot(view_dir, reflect_dir)), 32);
	white.r = 255;
	white.g = 255;
	white.b = 255;
	return (color_scale(white, spec * light_intensity * 0.5f));
}

void	clamp_color(t_color *color)
{
	color->r = fmin(255, fmax(0, color->r));
	color->g = fmin(255, fmax(0, color->g));
	color->b = fmin(255, fmax(0, color->b));
}
