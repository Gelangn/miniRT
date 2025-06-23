/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_lighting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:40:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/23 11:16:45 by anavas-g         ###   ########.fr       */
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

t_vector	setup_reflection_ray(t_global *global)
{
	t_vector	normal;
	t_vector	reflect_dir;
	t_vector	ray_origin;

	normal = get_surface_normal(global, global->c_ray.hit);
	reflect_dir = reflect_ray(global->c_ray.dir, normal);
	ray_origin = add(global->c_ray.hit.point, multiply(normal, EPSILON));
	global->c_ray.origin = ray_origin;
	global->c_ray.dir = reflect_dir;
	return (reflect_dir);
}

t_vector	setup_transmission_ray(t_global *global)
{
	t_vector	transmission_dir;
	t_vector	ray_origin;

	transmission_dir = global->c_ray.dir;
	ray_origin = add(global->c_ray.hit.point, multiply(transmission_dir, EPSILON));
	global->c_ray.origin = ray_origin;
	global->c_ray.dir = transmission_dir;
	return (transmission_dir);
}

t_color	apply_reflection_to_color(t_color final, t_color reflect, float reflct)
{
	t_color	result;

	result.r = final.r * (1.0f - reflct) + reflect.r * reflct;
	result.g = final.g * (1.0f - reflct) + reflect.g * reflct;
	result.b = final.b * (1.0f - reflct) + reflect.b * reflct;
	return (result);
}

t_color	cal_reflection(t_global *global, float reflct)
{
	t_intersec		reflect_hit;
	t_color			reflect_color;

	save_ray_state(global);
	setup_reflection_ray(global);
	reflect_hit = find_closest_isec(global);
	if (reflect_hit.obj_type >= 0)
	{
		global->c_ray.hit = reflect_hit;
		reflect_color = cal_lighting(global);
		reflect_color = color_scale(reflect_color, reflct);
	}
	else
		reflect_color = (t_color){20, 20, 20};
	restore_ray_state(global);
	return (reflect_color);
}

t_color	cal_transp(t_global *global, float transp)
{
	t_intersec		trans_hit;
	t_color			trans_color;

	save_ray_state(global);
	setup_transmission_ray(global);
	trans_hit = find_closest_isec(global);
	if (trans_hit.obj_type >= 0)
	{
		global->c_ray.hit = trans_hit;
		trans_color = cal_lighting(global);
		trans_color = color_scale(trans_color, transp);
	}
	else
	{
		trans_color.r = (BACKGROUND_COLOR >> 16) & 0xFF;
		trans_color.g = (BACKGROUND_COLOR >> 8) & 0xFF;
		trans_color.b = BACKGROUND_COLOR & 0xFF;
	}
	restore_ray_state(global);
	return (trans_color);
}

t_color	mix_colors_by_transp(t_color basic, t_color trans, float transp)
{
	t_color	final_color;
	float	basic_factor;
	float	trans_factor;

	if (transp > 0.7f)
	{
		basic_factor = 0.1f;
		trans_factor = 0.9f;
	}
	else if (transp > 0.5f)
	{
		basic_factor = 0.3f;
		trans_factor = 0.7f;
	}
	else
		return (basic);
	final_color.r = basic.r * basic_factor + trans.r * trans_factor;
	final_color.g = basic.g * basic_factor + trans.g * trans_factor;
	final_color.b = basic.b * basic_factor + trans.b * trans_factor;
	return (final_color);
}

t_color	cal_lighting_advanced(t_global *global)
{
	t_color_mix	colors;
	t_color		final_color;

	if (!is_valid_isec(global))
		return ((t_color){5, 5, 5});
	colors.transp = get_object_transp(global, global->c_ray.hit);
	colors.reflct = get_object_reflct(global, global->c_ray.hit);
	colors.basic = cal_lighting(global);
	if (colors.transp > 0.01f)
	{
		colors.trans = cal_transp(global, colors.transp);
		final_color = mix_colors_by_transp(colors.basic, colors.trans, colors.transp);
	}
	else
		final_color = colors.basic;
	if (colors.reflct > 0.01f)
	{
		colors.reflect = cal_reflection(global, colors.reflct);
		final_color = apply_reflection_to_color(final_color, colors.reflect, colors.reflct);
	}
	clamp_color(&final_color);
	return (final_color);
}
