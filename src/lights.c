/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:40:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 17:41:12 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	prepare_lighting_data(t_global *global)
{
	global->current_object_color = get_object_color(global);
	global->current_normal = get_surface_normal(global,
			global->current_intersec);
	global->current_light_dir = normalize(subtract(global->scene.light.pos,
				global->current_intersec.point));
	global->current_light_distance = magnitude(subtract(global->scene.light.pos,
				global->current_intersec.point));
}

t_color	apply_lighting(t_global *global, int in_shadow)
{
	t_color	result_color;
	t_color	diffuse;
	t_color	specular;

	result_color = calculate_ambient(global);
	if (in_shadow)
	{
		diffuse = calculate_diffuse(global);
		specular = calculate_specular(global);
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

	if (!is_valid_intersec(global))
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
