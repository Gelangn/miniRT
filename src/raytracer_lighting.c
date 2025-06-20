/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_lighting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:40:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/21 14:04:59 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

/**
 * Prepares all necessary data for lighting calculations
 * Extracts object color, calculates surface normal,
	and computes light direction
 * Also calculates the distance from intersection point to light source
 * 
 * @param global Structure containing scene data and current ray information
 */
void	prepare_lighting_data(t_global *global)
{
	global->c_ray.obj_color = get_object_color(global);
	global->c_ray.normal = get_surface_normal(global, global->c_ray.hit);
	global->c_light.dir = norm(subtract(global->scene.light.pos,
				global->c_ray.hit.point));
	global->c_light.distance = mag(subtract(global->scene.light.pos,
				global->c_ray.hit.point));
}

/**
 * Applies lighting calculations based on shadow information
 * Combines ambient, diffuse, and specular lighting components
 * Skips diffuse and specular calculations if the point is in shadow
 * 
 * @param global Structure containing lighting and scene data
 * @param in_shadow Flag indicating whether the point is in shadow
 * @return Final color after applying all lighting effects
 */
t_color	apply_lighting(t_global *global, int in_shadow)
{
	t_color	result_color;
	t_color	diffuse;
	t_color	specular;

	result_color = cal_ambient(global);
	if (!in_shadow)
	{
		diffuse = cal_diffuse(global);
		specular = cal_specular(global);
		result_color = color_add(result_color, diffuse);
		result_color = color_add(result_color, specular);
	}
	clamp_color(&result_color);
	return (result_color);
}

/**
 * Main lighting calculation function that coordinates the entire 
    lighting process
 * Validates intersection data, prepares lighting information,
	and applies lighting
 * Returns a default dark color for invalid intersections
 * 
 * @param global Structure containing all scene and ray data
 * @return Final calculated color for the intersection point
 */
t_color	cal_lighting(t_global *global)
{
	float	shadow_factor;
	t_color	default_color;

	if (!is_valid_isec(global))
	{
		default_color.r = 5;
		default_color.g = 5;
		default_color.b = 5;
		return (default_color);
	}
	prepare_lighting_data(global);
	shadow_factor = cal_shadow(global);
	return (apply_lighting(global, shadow_factor));
}
