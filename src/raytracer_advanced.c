/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_advanced.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:36:23 by bde-mada          #+#    #+#             */
/*   Updated: 2025/06/21 12:44:13 by anavas-g         ###   ########.fr       */
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
 * Prepares material properties for ray calculations
 */
static void	prepare_material_data(t_global *global, float *transp,
                float *reflct, float *fresnel, float *n1, float *n2)
{
    t_vector	normal;
    float		cos_angle;
    int			inside;

    *transp = get_object_transp(global, global->c_ray.hit);
    *reflct = get_object_reflct(global, global->c_ray.hit);
    normal = get_surface_normal(global, global->c_ray.hit);
    
    inside = is_inside_object(global, global->c_ray.hit, global->c_ray.origin);
    if (inside)
    {
        *n1 = get_object_refractive_index(global, global->c_ray.hit);
        *n2 = AIR_REFRACTIVE_INDEX;
    }
    else
    {
        *n1 = AIR_REFRACTIVE_INDEX;
        *n2 = get_object_refractive_index(global, global->c_ray.hit);
    }
    
    cos_angle = -dot(normal, global->c_ray.dir);
    *fresnel = schlick(cos_angle, *n1, *n2);
}

/**
 * Adds reflection ray to ray array
 */
static int	add_reflection_ray(t_global *global, float reflectivity,
                t_ray_result *next, int count)
{
    t_vector	normal;
    t_vector	reflect_dir;
    
    if (reflectivity < 0.02f)
        return (count);
        
    normal = get_surface_normal(global, global->c_ray.hit);
    reflect_dir = reflect_ray(global->c_ray.dir, normal);
    next[count].origin = add(global->c_ray.hit.point,
                    multiply(reflect_dir, EPSILON));
    next[count].direction = reflect_dir;
    next[count].contribution = reflectivity;
    return (count + 1);
}

/**
 * Adds refraction ray to ray array with adjusted contribution
 */
static int	add_refraction_ray(t_global *global, float transparency,
                float fresnel, float n1, float n2, t_ray_result *next, int count)
{
    t_vector	normal;
    t_vector	refract_dir;
    float		edge_factor;
    
    if (transparency < 0.05f)
        return (count);
    
    normal = get_surface_normal(global, global->c_ray.hit);
    refract_dir = refract_ray(global->c_ray.dir, normal, n1, n2);
    next[count].origin = add(global->c_ray.hit.point,
                    multiply(refract_dir, EPSILON));
    next[count].direction = refract_dir;
    
    // Factor de borde menos agresivo para cilindros
    edge_factor = 1.0f - fresnel;
    if (global->c_ray.hit.obj_type == 2)  // Si es un cilindro (valor correcto)
        edge_factor = fmax(edge_factor, 0.4f);  // Mantener más transparencia
    else if (edge_factor < 0.3f)
        edge_factor *= 0.8f;  // Menos agresivo que antes
        
    next[count].contribution = transparency * edge_factor;
    return (count + 1);
}

/**
 * Gets reflection and refraction properties for materials
 */
int	generate_secondary_rays(t_global *global, t_ray_result *next)
{
    float		transparency;
    float		reflectivity;
    float		fresnel;
    float		n1;
    float		n2;
    int			count;
    
    count = 0;
    prepare_material_data(global, &transparency, &reflectivity, 
            &fresnel, &n1, &n2);
    
    if (transparency < 0.05f && reflectivity < 0.02f)
        return (0);
    
    // Para objetos muy reflectivos, aumentar la reflectividad en los bordes
    if (reflectivity >= 0.5f)
        reflectivity = reflectivity + ((1.0f - reflectivity) * fresnel);
    
    count = add_reflection_ray(global, reflectivity, next, count);
    count = add_refraction_ray(global, transparency, fresnel, n1, n2, 
            next, count);
    
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
    t_ray_result    current_level[MAX_RAY_DEPTH * 2];
    t_ray_result    next_level[MAX_RAY_DEPTH * 2];
    t_color         accum_color;
    int             curr_count;
    int             next_count;
    int             depth;
    int             real_max_depth;

    // Inicializar arrays completamente a cero
    ft_memset(current_level, 0, sizeof(current_level));
    ft_memset(next_level, 0, sizeof(next_level));
    
    real_max_depth = max_depth;
    if (real_max_depth > MAX_RAY_DEPTH)
        real_max_depth = MAX_RAY_DEPTH;
        
    // Inicializar solo el primer rayo después del memset
    init_ray_data(current_level, origin, direction);
    accum_color = (t_color){0, 0, 0};
    curr_count = 1;
    depth = 0;
    
    while (depth < real_max_depth && curr_count > 0)
    {
        process_level_rays(global, current_level, curr_count, &accum_color);
        
        if (depth >= real_max_depth - 1)
            break;
            
        next_count = prepare_next_level(global, current_level, curr_count, next_level);
        
        if (next_count == 0 || !has_significant_contrib(next_level, next_count))
            break;
            
        copy_next_to_current(current_level, next_level, next_count);
        curr_count = next_count;
        depth++;
    }
    return (accum_color);
}