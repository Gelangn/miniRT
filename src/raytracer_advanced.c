/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_advanced.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-mada <bde-mada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:36:23 by bde-mada          #+#    #+#             */
/*   Updated: 2025/06/18 18:42:46 by bde-mada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_color	process_ray_level(t_global *global, t_ray_result *rays, int level_count)
{
    t_color			final_color;
    t_color			level_color;
    int				i;

    final_color = (t_color){0, 0, 0};
    i = -1;
    while (++i < level_count)
    {
        global->c_ray.origin = rays[i].origin;
        global->c_ray.dir = rays[i].direction;
        global->c_ray.hit = find_closest_isec(global);
        
        if (global->c_ray.hit.obj_type >= 0)
        {
            level_color = cal_lighting(global);
            level_color = color_scale(level_color, rays[i].contribution);
            final_color = color_add(final_color, level_color);
        }
    }
    return (final_color);
}

static int	generate_secondary_rays(t_global *global, t_ray_result *current_rays, 
                                int current_count, t_ray_result *next_rays)
{
    int				i;
    int				next_count;
    t_vector		reflect_dir;
    t_vector		refract_dir;
    float			transparency;
    float			reflectivity;
    float			fresnel;

    next_count = 0;
    i = -1;
    while (++i < current_count && next_count < MAX_RAY_DEPTH * 2)
    {
        global->c_ray.origin = current_rays[i].origin;
        global->c_ray.dir = current_rays[i].direction;
        global->c_ray.hit = find_closest_isec(global);
        
        if (global->c_ray.hit.obj_type < 0 || current_rays[i].contribution < MIN_CONTRIBUTION)
            continue;
        
        transparency = get_object_transparency(global, global->c_ray.hit);
        reflectivity = get_object_reflectivity(global, global->c_ray.hit);
        
        if (reflectivity > 0.01f)
        {
            global->c_ray.normal = get_surface_normal(global, global->c_ray.hit);
            reflect_dir = reflect_ray(global->c_ray.dir, global->c_ray.normal);
            
            next_rays[next_count].origin = add(global->c_ray.hit.point, 
                                            multiply(global->c_ray.normal, EPSILON));
            next_rays[next_count].direction = reflect_dir;
            next_rays[next_count].contribution = current_rays[i].contribution * reflectivity;
            next_rays[next_count].depth = current_rays[i].depth + 1;
            next_count++;
        }
        
        if (transparency > 0.01f)
        {
            float n1 = current_rays[i].is_inside ? 
                        get_object_refractive_index(global, global->c_ray.hit) : AIR_REFRACTIVE_INDEX;
            float n2 = current_rays[i].is_inside ? 
                        AIR_REFRACTIVE_INDEX : get_object_refractive_index(global, global->c_ray.hit);
            
            global->c_ray.normal = get_surface_normal(global, global->c_ray.hit);
            if (current_rays[i].is_inside)
                global->c_ray.normal = multiply(global->c_ray.normal, -1.0f);
            
            refract_dir = refract_ray(global->c_ray.dir, global->c_ray.normal, n1, n2);
            
            if (mag(refract_dir) > 0.1f)  // Check if refraction occurred
            {
                fresnel = fresnel_reflectance(global->c_ray.dir, global->c_ray.normal, n1, n2);
                
                next_rays[next_count].origin = add(global->c_ray.hit.point, 
                                                multiply(refract_dir, EPSILON));
                next_rays[next_count].direction = refract_dir;
                next_rays[next_count].contribution = current_rays[i].contribution * 
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
 */
t_color	trace_ray_iterative(t_global *global, t_vector origin, t_vector direction, int max_depth)
{
    t_ray_result	current_level[MAX_RAY_DEPTH * 2];
    t_ray_result	next_level[MAX_RAY_DEPTH * 2];
    t_color			accumulated_color;
    t_color			level_contribution;
    int				current_count;
    int				next_count;
    int				depth;

    // Initialize primary ray
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
        level_contribution = process_ray_level(global, current_level, current_count);
        accumulated_color = color_add(accumulated_color, level_contribution);
        
        next_count = generate_secondary_rays(global, current_level, current_count, next_level);
        
        // Swap levels
        int i = -1;
        while (++i < next_count)
            current_level[i] = next_level[i];
        current_count = next_count;
    }
    
    clamp_color(&accumulated_color);
    return (accumulated_color);
}