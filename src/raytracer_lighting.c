/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_lighting.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-mada <bde-mada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:40:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/18 18:40:53 by bde-mada         ###   ########.fr       */
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

t_color	cal_lighting_advanced(t_global *global)
{
    t_color	base_color;
    t_color	reflection_color;
    t_color	transmission_color;
    float	transparency;
    float	reflectivity;

    if (!is_valid_isec(global))
        return ((t_color){5, 5, 5});

    // Calculate base lighting (ambient + diffuse + specular)
    prepare_lighting_data(global);
    base_color = apply_lighting(global, cal_shadow(global));
    
    transparency = get_object_transparency(global, global->c_ray.hit);
    reflectivity = get_object_reflectivity(global, global->c_ray.hit);
    
    // If object has no special properties, return basic lighting
    if (transparency < 0.01f && reflectivity < 0.01f)
        return (base_color);
    
    // Calculate reflections and refractions
    reflection_color = (t_color){0, 0, 0};
    transmission_color = (t_color){0, 0, 0};
    
    if (reflectivity > 0.01f || transparency > 0.01f)
    {
        t_vector reflect_origin = add(global->c_ray.hit.point, 
                                    multiply(global->c_ray.normal, EPSILON));
        reflection_color = trace_ray_iterative(global, reflect_origin, 
                                            global->c_ray.dir, MAX_RAY_DEPTH - 1);
    }
    
    // Blend colors based on material properties
    base_color = color_scale(base_color, 1.0f - transparency - reflectivity);
    reflection_color = color_scale(reflection_color, reflectivity);
    transmission_color = color_scale(transmission_color, transparency);
    
    t_color final_color = color_add(base_color, reflection_color);
    final_color = color_add(final_color, transmission_color);
    
    clamp_color(&final_color);
    return (final_color);
}
