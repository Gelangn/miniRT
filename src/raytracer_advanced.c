/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_advanced.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:36:23 by bde-mada          #+#    #+#             */
/*   Updated: 2025/06/19 14:32:53 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_color	process_ray_level(t_global *global, t_ray_result *rays,
		int level_count)
{
	t_color	final_color;
	t_color	level_color;
	int		i;
	float	transparency;
	float	reflectivity;

	final_color = (t_color){0, 0, 0};
	i = -1;
	while (++i < level_count)
	{
		global->c_ray.origin = rays[i].origin;
		global->c_ray.dir = rays[i].direction;
		global->c_ray.hit = find_closest_isec(global);
		if (global->c_ray.hit.obj_type >= 0)
		{
			// Use advanced lighting if object has transparency or reflectivity
			transparency = get_object_transparency(global, global->c_ray.hit);
			reflectivity = get_object_reflectivity(global, global->c_ray.hit);
			if (transparency > 0.01f || reflectivity > 0.01f)
				level_color = cal_lighting_advanced(global);
			else
				level_color = cal_lighting(global);
			level_color = color_scale(level_color, rays[i].contribution);
			final_color = color_add(final_color, level_color);
		}
	}
	return (final_color);
}

static int	generate_secondary_rays(t_global *global,
									t_ray_result *current_rays,
									int current_count,
									t_ray_result *next_rays)
{
	int			i;
	int			next_count;
	t_vector	reflect_dir;
	t_vector	refract_dir;
	float		transparency;
	float		reflectivity;
	float		fresnel;
	float		n1;
	float		n2;

	// Initialize all variables to prevent uninitialized value usage
	next_count = 0;
	reflect_dir = (t_vector){0, 0, 0};
	refract_dir = (t_vector){0, 0, 0};
	transparency = 0.0f;
	reflectivity = 0.0f;
	fresnel = 0.0f;
	n1 = 0.0f;
	n2 = 0.0f;
	// Initialize next_rays array
	ft_memset(next_rays, 0, sizeof(t_ray_result) * MAX_RAY_DEPTH * 2);
	i = -1;
	while (++i < current_count && next_count < MAX_RAY_DEPTH * 2)
	{
		// Initialize ray for this iteration
		global->c_ray.origin = current_rays[i].origin;
		global->c_ray.dir = current_rays[i].direction;
		global->c_ray.hit = find_closest_isec(global);
		if (global->c_ray.hit.obj_type < 0
			|| current_rays[i].contribution < MIN_CONTRIBUTION)
			continue ;
		transparency = get_object_transparency(global, global->c_ray.hit);
		reflectivity = get_object_reflectivity(global, global->c_ray.hit);
		if (reflectivity > 0.01f && next_count < MAX_RAY_DEPTH * 2 - 1)
		{
			global->c_ray.normal = get_surface_normal(global,
														global->c_ray.hit);
			reflect_dir = reflect_ray(global->c_ray.dir, global->c_ray.normal);
			// Fully initialize reflection ray
			next_rays[next_count].origin = add(global->c_ray.hit.point,
												multiply(global->c_ray.normal,
														EPSILON));
			next_rays[next_count].direction = reflect_dir;
			next_rays[next_count].contribution = current_rays[i].contribution
				* reflectivity;
			next_rays[next_count].depth = current_rays[i].depth + 1;
			next_rays[next_count].is_inside = current_rays[i].is_inside;
			next_count++;
		}
		if (transparency > 0.01f && next_count < MAX_RAY_DEPTH * 2 - 1)
		{
			n1 = current_rays[i].is_inside ? get_object_refractive_index(global,
																			global->c_ray.hit)
											: AIR_REFRACTIVE_INDEX;
			n2 = current_rays[i].is_inside ? AIR_REFRACTIVE_INDEX : get_object_refractive_index(global,
					global->c_ray.hit);
			global->c_ray.normal = get_surface_normal(global,
														global->c_ray.hit);
			if (current_rays[i].is_inside)
				global->c_ray.normal = multiply(global->c_ray.normal, -1.0f);
			refract_dir = refract_ray(global->c_ray.dir, global->c_ray.normal,
					n1, n2);
			if (mag(refract_dir) > 0.1f)
			{
				fresnel = fresnel_reflectance(global->c_ray.dir,
												global->c_ray.normal,
												n1,
												n2);
				// Fully initialize refraction ray
				next_rays[next_count].origin = add(global->c_ray.hit.point,
													multiply(refract_dir,
															EPSILON));
				next_rays[next_count].direction = refract_dir;
				next_rays[next_count].contribution = current_rays[i].contribution
					*
					transparency * (1.0f - fresnel);
				next_rays[next_count].depth = current_rays[i].depth + 1;
				next_rays[next_count].is_inside = !current_rays[i].is_inside;
				next_count++;
			}
		}
	}
	return (next_count);
}

/**
 * Iterative ray tracing with depth limiting
 * Processes multiple ray levels without deep recursion
 * Uses proper memory management with finish() for error handling
 */
t_color	trace_ray_iterative(t_global *global, t_vector origin,
		t_vector direction, int max_depth)
{
	t_ray_result current_level[MAX_RAY_DEPTH * 2];
	t_ray_result next_level[MAX_RAY_DEPTH * 2];
	t_color accumulated_color;
	t_color level_contribution;
	int current_count;
	int next_count;
	int depth;

	// Initialize all arrays completely
	ft_memset(current_level, 0, sizeof(current_level));
	ft_memset(next_level, 0, sizeof(next_level));

	// Initialize primary ray with all fields
	current_level[0].origin = origin;
	current_level[0].direction = direction;
	current_level[0].contribution = 1.0f;
	current_level[0].depth = 0;
	current_level[0].is_inside = 0;
	current_count = 1;

	accumulated_color = (t_color){0, 0, 0};
	depth = -1;
	while (++depth < max_depth && current_count > 0)
	{
		level_contribution = process_ray_level(global, current_level,
				current_count);
		accumulated_color = color_add(accumulated_color, level_contribution);

		// Only generate secondary rays if we have more depth to go
		if (depth < max_depth - 1)
		{
			next_count = generate_secondary_rays(global, current_level,
					current_count, next_level);
			if (next_count <= 0 || next_count > MAX_RAY_DEPTH * 2)
				break ;

			// Safely copy rays for next iteration
			ft_memcpy(current_level, next_level, sizeof(t_ray_result)
					* next_count);
			current_count = next_count;
		}
		else
			break ;
	}

	// If no color accumulated, return basic lighting
	if (accumulated_color.r < 1 && accumulated_color.g < 1
		&& accumulated_color.b < 1)
		return (cal_lighting(global));

	clamp_color(&accumulated_color);
	return (accumulated_color);
}