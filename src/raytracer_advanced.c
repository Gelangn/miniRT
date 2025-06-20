/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_advanced.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:36:23 by bde-mada          #+#    #+#             */
/*   Updated: 2025/06/20 18:12:47 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

#define MAX_RAYS 100

/**
 * Initializes ray data for iterative tracing
 */
static void	init_ray_data(t_ray_result *current, t_vector origin,
            t_vector direction)
{
    current[0].origin = origin;
    current[0].direction = direction;
    current[0].contribution = 1.0f;
    current[0].depth = 0;
    current[0].is_inside = 0;
}

/**
 * Processes a single ray and returns color
 */
static t_color	process_single_ray(t_global *global, t_ray_result *ray)
{
    t_color	ray_color;

    global->c_ray.origin = ray->origin;
    global->c_ray.dir = ray->direction;
    global->c_ray.hit = find_closest_isec(global);
    
    if (global->c_ray.hit.obj_type >= 0)
        ray_color = cal_lighting(global);
    else
        ray_color = (t_color){0, 0, 0}; // Color de fondo negro por defecto
    
    return (ray_color);
}

/**
 * Processes all rays at current depth level
 */
static t_color	process_level_rays(t_global *global, t_ray_result *rays,
            int count, t_color *accum)
{
    t_color	level_contrib;
    t_color	ray_color;
    int		i;

    level_contrib = (t_color){0, 0, 0};
    i = 0;
    while (i < count)
    {
        ray_color = process_single_ray(global, &rays[i]);
        ray_color = color_scale(ray_color, rays[i].contribution);
        level_contrib = color_add(level_contrib, ray_color);
        i++;
    }
    *accum = color_add(*accum, level_contrib);
    return (level_contrib);
}

/**
 * Gets reflection and refraction properties for materials
 */
int	generate_secondary_rays(t_global *global, t_ray_result *next)
{
    float		transparency;
    float		reflectivity;
    t_vector	normal;
    t_vector	reflect_dir;
    int			count;

    count = 0;
    transparency = get_object_transp(global, global->c_ray.hit);
    reflectivity = get_object_reflct(global, global->c_ray.hit);
    
    if (transparency < 0.01f && reflectivity < 0.01f)
        return (0);
        
    normal = get_surface_normal(global, global->c_ray.hit);
    
    if (reflectivity >= 0.01f)
    {
        reflect_dir = reflect_ray(global->c_ray.dir, normal);
        next[count].origin = add(global->c_ray.hit.point, 
                           multiply(reflect_dir, EPSILON));
        next[count].direction = reflect_dir;
        next[count].contribution = reflectivity;
        count++;
    }
    
    return (count);
}

/**
 * Generates secondary rays for next depth level
 */
static int	prepare_next_level(t_global *global, t_ray_result *curr,
            int curr_cnt, t_ray_result *next)
{
    int	i;
    int	j;
    int	next_count;
    int	new_rays;

    next_count = 0;
    i = 0;
    while (i < curr_cnt)
    {
        global->c_ray.origin = curr[i].origin;
        global->c_ray.dir = curr[i].direction;
        global->c_ray.hit = find_closest_isec(global);
        
        if (global->c_ray.hit.obj_type >= 0)
        {
            new_rays = generate_secondary_rays(global, &next[next_count]);
            j = 0;
            while (j < new_rays)
            {
                next[next_count + j].contribution *= curr[i].contribution;
                next[next_count + j].depth = curr[i].depth + 1;
                j++;
            }
            next_count += new_rays;
        }
        i++;
    }
    return (next_count);
}

/**
 * Copies next level rays to current level array
 */
static void	copy_next_to_current(t_ray_result *curr, t_ray_result *next,
            int count)
{
    int	i;

    i = 0;
    while (i < count)
    {
        curr[i] = next[i];
        i++;
    }
}

/**
 * Checks if any ray in the level has significant contribution
 */
static int	has_significant_contrib(t_ray_result *rays, int count)
{
    int	i;

    i = 0;
    while (i < count)
    {
        if (rays[i].contribution > MIN_CONTRIBUTION)
            return (1);
        i++;
    }
    return (0);
}

/**
 * Traces rays iteratively through the scene with multiple bounces
 */
t_color	trace_ray_iterative(t_global *global, t_vector origin, 
        t_vector direction, int max_depth)
{
    t_ray_result	current[MAX_RAYS];
    t_ray_result	next[MAX_RAYS];
    t_color			accum_color;
    int				curr_count;
    int				next_count;
    int				depth;
    int             real_max_depth;

    real_max_depth = max_depth;
    if (real_max_depth > MAX_RAY_DEPTH)
        real_max_depth = MAX_RAY_DEPTH;
        
    init_ray_data(current, origin, direction);
    accum_color = (t_color){0, 0, 0};
    curr_count = 1;
    depth = 0;
    
    while (depth < real_max_depth && curr_count > 0)
    {
        process_level_rays(global, current, curr_count, &accum_color);
        
        if (depth >= real_max_depth - 1)
            break;
            
        next_count = prepare_next_level(global, current, curr_count, next);
        
        if (next_count == 0 || !has_significant_contrib(next, next_count))
            break;
            
        copy_next_to_current(current, next, next_count);
        curr_count = next_count;
        depth++;
    }
    return (accum_color);
}