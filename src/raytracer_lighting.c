/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_lighting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:40:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/26 13:43:49 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

// Prepares lighting data such as object color, normal, and light direction
void	prepare_lighting_data(t_global *global)
{
	global->c_ray.obj_color = get_object_color(global);
	global->c_ray.normal = get_surface_normal(global, global->c_ray.hit);
	global->c_light.dir = norm(subtract(global->scene.light.pos,
										global->c_ray.hit.point));
	global->c_light.distance = mag(subtract(global->scene.light.pos,
											global->c_ray.hit.point));
}

// Scales color components based on shadow factor
static t_color	scale_color_by_shadow(t_color color, float shadow_factor)
{
	color.r = (int)(color.r * (1.0f - shadow_factor));
	color.g = (int)(color.g * (1.0f - shadow_factor));
	color.b = (int)(color.b * (1.0f - shadow_factor));
	return (color);
}

// Applies lighting effects including ambient, diffuse, and specular
t_color	apply_lighting(t_global *global, float shadow_factor)
{
	t_color	result_color;
	t_color	diffuse;
	t_color	specular;

	result_color = cal_ambient(global);
	if (!comp_floats(shadow_factor, 1.0f))
	{
		diffuse = scale_color_by_shadow(cal_diffuse(global), shadow_factor);
		specular = scale_color_by_shadow(cal_specular(global), shadow_factor);
		result_color.r += diffuse.r + specular.r;
		result_color.g += diffuse.g + specular.g;
		result_color.b += diffuse.b + specular.b;
	}
	else if (comp_floats(shadow_factor, 1.0f))
		result_color = cal_ambient(global);
	clamp_color(&result_color);
	return (result_color);
}

// Calculates lighting for the current ray intersection
t_color	cal_lighting(t_global *global)
{
	float	shadow_factor;

	if (!is_valid_isec(global))
		return ((t_color){5, 5, 5});
	prepare_lighting_data(global);
	shadow_factor = cal_shadow(global);
	return (apply_lighting(global, shadow_factor));
}

// Sets up reflection ray state
static void	setup_reflection_ray_state(t_global *global, t_vector *reflect_dir,
	t_vector *ray_origin, t_vector *normal)
{
	*normal = get_surface_normal(global, global->c_ray.hit);
	*reflect_dir = reflect_ray(global->c_ray.dir, *normal);
	*ray_origin = add(global->c_ray.hit.point, multiply(*normal, EPSILON));
}

// Restores reflection ray state
static void	restore_reflection_ray_state(t_global *global, t_intersec old_hit,
	t_vector old_origin, t_vector old_dir)
{
	global->c_ray.hit = old_hit;
	global->c_ray.origin = old_origin;
	global->c_ray.dir = old_dir;
}

// Calculates reflection contribution by casting a reflection ray
static t_color	calculate_reflection(t_global *global, float reflectivity)
{
	t_vector	reflect_dir;
	t_vector	ray_origin;
	t_vector	normal;
	t_intersec	old_hit;
	t_vector	old_origin;

	setup_reflection_ray_state(global, &reflect_dir, &ray_origin, &normal);
	old_hit = global->c_ray.hit;
	old_origin = global->c_ray.origin;
	global->c_ray.origin = ray_origin;
	global->c_ray.dir = reflect_dir;
	t_intersec reflect_hit = find_closest_isec(global);
	if (reflect_hit.obj_type >= 0)
	{
		global->c_ray.hit = reflect_hit;
		restore_reflection_ray_state(global, old_hit, old_origin, global->c_ray.dir);
		return (color_scale(cal_lighting(global), reflectivity));
	}
	restore_reflection_ray_state(global, old_hit, old_origin, global->c_ray.dir);
	return ((t_color){20, 20, 20});
}

// Sets up transmission ray state
static void	setup_transmission_ray_state(t_global *global, t_vector *transmission_dir,
	t_vector *ray_origin)
{
	*transmission_dir = global->c_ray.dir;
	*ray_origin = add(global->c_ray.hit.point, multiply(*transmission_dir, EPSILON));
}

// Restores transmission ray state
static void	restore_transmission_ray_state(t_global *global, t_intersec old_hit,
	t_vector old_origin, t_vector old_dir)
{
	global->c_ray.hit = old_hit;
	global->c_ray.origin = old_origin;
	global->c_ray.dir = old_dir;
}

// Calculates transparency contribution by casting a transmission ray
static t_color	calculate_transparency(t_global *global, float transparency)
{
	t_vector	transmission_dir;
	t_vector	ray_origin;
	t_intersec	old_hit;
	t_vector	old_origin;

	setup_transmission_ray_state(global, &transmission_dir, &ray_origin);
	old_hit = global->c_ray.hit;
	old_origin = global->c_ray.origin;
	global->c_ray.origin = ray_origin;
	global->c_ray.dir = transmission_dir;
	t_intersec trans_hit = find_closest_isec(global);
	if (trans_hit.obj_type >= 0)
	{
		global->c_ray.hit = trans_hit;
		restore_transmission_ray_state(global, old_hit, old_origin, global->c_ray.dir);
		return (color_scale(cal_lighting(global), transparency));
	}
	restore_transmission_ray_state(global, old_hit, old_origin, global->c_ray.dir);
	return ((t_color){
		(BACKGROUND_COLOR >> 16) & 0xFF,
		(BACKGROUND_COLOR >> 8) & 0xFF,
		BACKGROUND_COLOR & 0xFF});
}

// Handles transparency blending for advanced lighting
static t_color	handle_transparency(t_global *global, t_color basic_color,
	float transparency)
{
	t_color	trans_color;
	t_color	final_color;

	if (transparency > 0.7f)
	{
		final_color = color_scale(basic_color, 0.1f);
		trans_color = calculate_transparency(global, transparency);
		final_color.r += trans_color.r * 0.9f;
		final_color.g += trans_color.g * 0.9f;
		final_color.b += trans_color.b * 0.9f;
	}
	else if (transparency > 0.5f)
	{
		final_color = color_scale(basic_color, 0.3f);
		trans_color = calculate_transparency(global, transparency);
		final_color.r += trans_color.r * 0.7f;
		final_color.g += trans_color.g * 0.7f;
		final_color.b += trans_color.b * 0.7f;
	}
	else
		final_color = basic_color;
	return (final_color);
}

// Calculates advanced lighting including reflection and refraction
t_color	cal_lighting_advanced(t_global *global)
{
	t_color	basic_color;
	t_color	final_color;
	t_color	reflect_color;
	float	transparency;
	float	reflectivity;

	if (!is_valid_isec(global))
		return ((t_color){5, 5, 5});
	transparency = get_object_transparency(global, global->c_ray.hit);
	reflectivity = get_object_reflectivity(global, global->c_ray.hit);
	basic_color = cal_lighting(global);
	final_color = handle_transparency(global, basic_color, transparency);
	if (reflectivity > 0.01f)
	{
		reflect_color = calculate_reflection(global, reflectivity);
		final_color.r = final_color.r * (1.0f - reflectivity) + reflect_color.r
			* reflectivity;
		final_color.g = final_color.g * (1.0f - reflectivity) + reflect_color.g
			* reflectivity;
		final_color.b = final_color.b * (1.0f - reflectivity) + reflect_color.b
			* reflectivity;
	}
	clamp_color(&final_color);
	return (final_color);
}
