/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_lighting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:40:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/20 17:42:43 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	prepare_lighting_data(t_global *global)
{
	global->c_ray.obj_color = get_object_color(global);
	global->c_ray.normal = get_surface_normal(global, global->c_ray.hit);
	global->c_light.dir = norm(subtract(global->scene.light.pos,
										global->c_ray.hit.point));
	global->c_light.distance = mag(subtract(global->scene.light.pos,
											global->c_ray.hit.point));
}

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

/**
 * Calculates reflection contribution by casting a reflection ray
 */
static t_color	cal_reflection(t_global *global, float reflct)
{
	t_vector	normal;
	t_vector	reflect_dir;
	t_vector	ray_origin;
	t_intersec	old_hit;
	t_vector	old_origin;
	t_vector	old_dir;
	t_intersec	reflect_hit;
	t_color		reflect_color;

	normal = get_surface_normal(global, global->c_ray.hit);
	reflect_dir = reflect_ray(global->c_ray.dir, normal);
	ray_origin = add(global->c_ray.hit.point, multiply(normal, EPSILON));
	// Save current ray state
	old_hit = global->c_ray.hit;
	old_origin = global->c_ray.origin;
	old_dir = global->c_ray.dir;
	// Cast reflection ray
	global->c_ray.origin = ray_origin;
	global->c_ray.dir = reflect_dir;
	reflect_hit = find_closest_isec(global);
	if (reflect_hit.obj_type >= 0)
	{
		global->c_ray.hit = reflect_hit;
		reflect_color = cal_lighting(global);
		reflect_color = color_scale(reflect_color, reflct);
	}
	else
		reflect_color = (t_color){20, 20, 20}; // Sky color
	// Restore original ray state
	global->c_ray.hit = old_hit;
	global->c_ray.origin = old_origin;
	global->c_ray.dir = old_dir;
	return (reflect_color);
}

/**
 * Calculates transparency contribution by casting a transmission ray
 */
static t_color	cal_transp(t_global *global, float transp)
{
	t_vector	transmission_dir;
	t_vector	ray_origin;
	t_intersec	old_hit;
	t_vector	old_origin;
	t_vector	old_dir;
	t_intersec	trans_hit;
	t_color		trans_color;

	// For simple transparency, just continue the ray in the same direction
	transmission_dir = global->c_ray.dir;
	ray_origin = add(global->c_ray.hit.point, multiply(transmission_dir,
				EPSILON));
	// Save current ray state
	old_hit = global->c_ray.hit;
	old_origin = global->c_ray.origin;
	old_dir = global->c_ray.dir;
	// Cast transmission ray
	global->c_ray.origin = ray_origin;
	global->c_ray.dir = transmission_dir;
	trans_hit = find_closest_isec(global);
	if (trans_hit.obj_type >= 0)
	{
		global->c_ray.hit = trans_hit;
		trans_color = cal_lighting(global);
		trans_color = color_scale(trans_color, transp);
	}
	else
	{
		// Convert BACKGROUND_COLOR to t_color structure
		trans_color.r = (BACKGROUND_COLOR >> 16) & 0xFF;
		trans_color.g = (BACKGROUND_COLOR >> 8) & 0xFF;
		trans_color.b = BACKGROUND_COLOR & 0xFF;
	}
	// Restore original ray state
	global->c_ray.hit = old_hit;
	global->c_ray.origin = old_origin;
	global->c_ray.dir = old_dir;
	return (trans_color);
}

/**
 * Advanced lighting calculation that includes reflection and refraction
 * Uses real ray tracing for transparency and reflection effects
 */
t_color	cal_lighting_advanced(t_global *global)
{
	t_color	basic_color;
	t_color	final_color;
	t_color	trans_color;
	float	transp;
	float	reflct;

	if (!is_valid_isec(global))
		return ((t_color){5, 5, 5});
	
	// Get material properties
	transp = get_object_transp(global, global->c_ray.hit);
	reflct = get_object_reflct(global, global->c_ray.hit);
	
	// Get basic lighting first
	basic_color = cal_lighting(global);
	
	// SIMPLE transp test - just make object darker based on transp
	if (transp > 0.7f)
	{
		// Very transparent - almost see-through
		final_color.r = basic_color.r * 0.1f;
		final_color.g = basic_color.g * 0.1f;
		final_color.b = basic_color.b * 0.1f;
		
		// Add background color
		trans_color = cal_transp(global, transp);
		final_color.r += trans_color.r * 0.9f;
		final_color.g += trans_color.g * 0.9f;
		final_color.b += trans_color.b * 0.9f;
	}
	else if (transp > 0.5f)
	{
		// Semi-transparent
		final_color.r = basic_color.r * 0.3f;
		final_color.g = basic_color.g * 0.3f;
		final_color.b = basic_color.b * 0.3f;
		
		trans_color = cal_transp(global, transp);
		final_color.r += trans_color.r * 0.7f;
		final_color.g += trans_color.g * 0.7f;
		final_color.b += trans_color.b * 0.7f;
	}
	else
	{
		final_color = basic_color;
	}
	
	// Add reflection if reflective
	if (reflct > 0.01f)
	{
		t_color reflect_color = cal_reflection(global, reflct);
		final_color.r = final_color.r * (1.0f - reflct) + reflect_color.r * reflct;
		final_color.g = final_color.g * (1.0f - reflct) + reflect_color.g * reflct;
		final_color.b = final_color.b * (1.0f - reflct) + reflect_color.b * reflct;
	}
	
	clamp_color(&final_color);
	return (final_color);
}
