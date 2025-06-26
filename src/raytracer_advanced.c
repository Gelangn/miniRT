/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_advanced.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:36:23 by bde-mada          #+#    #+#             */
/*   Updated: 2025/06/26 12:45:14 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static t_color	process_ray_level(t_global *global, t_ray_result *rays,
		int level_count)
{
	t_color	final_color;
	t_color	level_color;
	int		i;

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
			if (get_object_transparency(global, global->c_ray.hit) > 0.5f)
			{
				float edge_factor = 1.0f - fabs(dot(global->c_ray.dir, 
										 get_surface_normal(global, global->c_ray.hit)));
				edge_factor = edge_factor * edge_factor * 50.0f;
				level_color.r = fmin(255, level_color.r + edge_factor);
				level_color.g = fmin(255, level_color.g + edge_factor);
				level_color.b = fmin(255, level_color.b + edge_factor);
			}
			
			level_color = color_scale(level_color, rays[i].contribution);
			final_color = color_add(final_color, level_color);
			/* printf("Ray contribution: %.3f, Color: (%d,%d,%d)\n",
				rays[i].contribution, level_color.r, level_color.g, level_color.b); */
		}
	}
	return (final_color);
}

/**
 * Generates secondary rays for reflection and refraction
 */
int generate_secondary_rays(t_global *global, t_ray_result *rays, int *count)
{
    t_vector    normal;
    t_vector    reflect_dir;
    t_vector    refract_dir;
    float       transparency;
    float       reflectivity;
    float       n1, n2;
    float       fresnel;
    int         inside;
    
    transparency = get_object_transparency(global, global->c_ray.hit);
    reflectivity = get_object_reflectivity(global, global->c_ray.hit);
    
    if (transparency < MIN_CONTRIBUTION && reflectivity < MIN_CONTRIBUTION)
        return 0;
    
    normal = get_surface_normal(global, global->c_ray.hit);
    inside = is_inside_object(global, global->c_ray.hit, global->c_ray.origin);

    // Set refractive indices based on whether ray is entering or exiting
    if (inside) {
        n1 = get_object_refractive_index(global, global->c_ray.hit);
        n2 = AIR_REFRACTIVE_INDEX;
    } else {
        n1 = AIR_REFRACTIVE_INDEX;
        n2 = get_object_refractive_index(global, global->c_ray.hit);
    }
    
    // Calculate Fresnel effect for more realistic reflections
    fresnel = schlick(-dot(normal, global->c_ray.dir), n1, n2);
    
	printf("\n--- RAY INFO ---\n");
	printf("Hit object: type=%d (0=Sp 1=Pl 2=Cyl)\n", global->c_ray.hit.obj_type);
	printf("Transparency: %.2f\n", transparency);
	printf("¿Inside object? %s\n", inside ? "YES" : "NO");
	printf("Normal: (%.2f, %.2f, %.2f)\n", normal.x, normal.y, normal.z);

   /*  // Si el objeto es un cilindro
    if (global->c_ray.hit.obj_type == 2) {
        printf("CILINDRO: Punto=(%.2f,%.2f,%.2f), Inside=%d\n",
            global->c_ray.hit.point.x, 
            global->c_ray.hit.point.y,
            global->c_ray.hit.point.z,
            inside);
            
        // Para depurar is_inside_cylinder directamente
        t_cylinder *cyl = &global->scene.cyls[global->c_ray.hit.obj_index];
        int test_inside = is_inside_cylinder(cyl, global->c_ray.origin);
        printf("Test Inside directo: %d\n", test_inside);
        
        printf("CILINDRO: Normal=(%.2f,%.2f,%.2f)\n", 
            normal.x, normal.y, normal.z);
    } */
    
    // Adjust reflectivity using Fresnel (more reflection at grazing angles)
    reflectivity = reflectivity + (1.0f - reflectivity) * fresnel;
    
    // Add reflection ray
    if (reflectivity > MIN_CONTRIBUTION) {
        reflect_dir = reflect_ray(global->c_ray.dir, normal);
        rays[*count].origin = add(global->c_ray.hit.point, 
                                multiply(reflect_dir, EPSILON));
        rays[*count].direction = reflect_dir;
        rays[*count].contribution = reflectivity;
        (*count)++;
    }
    
    // Add refraction ray
    if (transparency > MIN_CONTRIBUTION) {
        refract_dir = refract_ray(global->c_ray.dir, normal, n1, n2);
        rays[*count].origin = add(global->c_ray.hit.point, 
                                multiply(refract_dir, EPSILON));
        rays[*count].direction = refract_dir;
        rays[*count].contribution = transparency * (1.0f - fresnel);
        (*count)++;
		printf("REFRACTION ray created: contrib=%.3f, dir=(%.2f,%.2f,%.2f)\n", 
			rays[*count-1].contribution, refract_dir.x, refract_dir.y, refract_dir.z);
    }
	
	/* // Después de calcular refract_dir
	if (global->c_ray.hit.obj_type == 2) {
		printf("CILINDRO: ¿Se generó rayo refractado? %s\n", 
			(fresnel < 1.0f) ? "SÍ" : "NO");
		printf("CILINDRO: Dirección refracción=(%.2f,%.2f,%.2f), Fresnel=%.3f\n",
			refract_dir.x, refract_dir.y, refract_dir.z, fresnel);
	} */
	
	return (*count);
}

/**
 * Iterative ray tracing with depth limiting
 * Processes multiple ray levels without deep recursion
 * Uses proper memory management with finish() for error handling
 */
t_color	trace_ray_iterative(t_global *global, t_vector origin,
        t_vector direction, int max_depth)
{
    // NUEVO: Código específico para cilindros con transparencia
    // Lanzar un rayo inicial y verificar si golpea un cilindro
    t_active_ray saved_ray = global->c_ray;
    global->c_ray.origin = origin;
    global->c_ray.dir = direction;
    t_intersec hit = find_closest_isec(global);
    
    // Si es un cilindro
    if (hit.obj_type == 2 && hit.obj_index >= 0 && 
        hit.obj_index < global->scene.num_cy) 
    {
        // Verificar si es transparente
        float transparency = global->scene.cyls[hit.obj_index].transparency;
        if (transparency > 0.1f) 
        {
            // Calcular color del cilindro
            global->c_ray.hit = hit;
            t_color cyl_color = cal_lighting(global);
            
            // Buscar lo que hay detrás
            t_vector through_point = add(hit.point, multiply(direction, 0.1f));
            global->c_ray.origin = through_point;
            t_intersec behind_hit = find_closest_isec(global);
            
            // Si hay algo detrás, mezclar colores
            if (behind_hit.obj_type >= 0) 
            {
                global->c_ray.hit = behind_hit;
                t_color behind_color = cal_lighting(global);
                
                // Restaurar rayo original
                global->c_ray = saved_ray;
                
                // Crear color mezclado
                t_color mixed;
                mixed.r = (int)(cyl_color.r * (1.0f - transparency) + 
                               behind_color.r * transparency);
                mixed.g = (int)(cyl_color.g * (1.0f - transparency) + 
                               behind_color.g * transparency);
                mixed.b = (int)(cyl_color.b * (1.0f - transparency) + 
                               behind_color.b * transparency);
                
                // Asegurar valores en rango
                if (mixed.r > 255) mixed.r = 255;
                if (mixed.g > 255) mixed.g = 255;
                if (mixed.b > 255) mixed.b = 255;
                
                return mixed;
            }
            
            // Restaurar estado original si no hay nada detrás
            global->c_ray = saved_ray;
        }
    }
    
    // Restaurar rayo y continuar con el código existente
    global->c_ray = saved_ray;

    t_ray_result current_level[MAX_RAY_DEPTH * 2];
    t_ray_result next_level[MAX_RAY_DEPTH * 2];
    t_color accumulated_color;
    t_color level_contribution;
    int current_count;
    int next_count;
    int depth;
	global->c_ray.origin = origin;
    global->c_ray.dir = direction;

	printf("DEBUG: trace_ray_iterative iniciado\n");
    
    if (hit.obj_type == 2) { // Es un cilindro
        float transparency = 0.0f;
        
        // Verificar si es transparente
        if (hit.obj_index >= 0 && hit.obj_index < global->scene.num_cy) {
            transparency = global->scene.cyls[hit.obj_index].transparency;
            //printf("CILINDRO transparencia: %.3f\n", transparency);
        }
        
        if (transparency > 0.3f) {
            //printf("APLICANDO TRANSPARENCIA FORZADA EN CILINDRO\n");
            
            // 1. Calcular el color del cilindro
            global->c_ray.hit = hit;
            t_color cylinder_color = cal_lighting(global);
            
            // 2. Calcular punto justo después del cilindro para un nuevo rayo
            t_vector through_point = add(hit.point, multiply(direction, 0.01f));
            
            // 3. Lanzar un nuevo rayo desde ese punto
            t_vector saved_origin = global->c_ray.origin;
            global->c_ray.origin = through_point;
            
            t_intersec next_hit = find_closest_isec(global);
            
            // 4. Si golpea algo, mezclar los colores
            if (next_hit.obj_type >= 0) {
                global->c_ray.hit = next_hit;
                t_color behind_color = cal_lighting(global);
                
                // Restaurar origen
                global->c_ray.origin = saved_origin;
                
                // Mezclar colores directamente
                t_color final;
                final.r = (int)(cylinder_color.r * (1.0f - transparency) + 
                                behind_color.r * transparency);
                final.g = (int)(cylinder_color.g * (1.0f - transparency) + 
                                behind_color.g * transparency);
                final.b = (int)(cylinder_color.b * (1.0f - transparency) + 
                                behind_color.b * transparency);
                
                // Asegurar que los valores estén en el rango correcto
                clamp_color(&final);
                return final;
            }
            
            // Restaurar en caso de no golpear nada
            global->c_ray.origin = saved_origin;
        }
    }
    
    // AGREGAR: variables para manejar la transparencia
    t_color surface_color = {0, 0, 0};
    float transparency = 0.0f;
    int found_transparency = 0;

    static int debug_x = -1;
    static int debug_y = -1;
    int debug_this_ray = (global->c_ray.hit.point.x == debug_x &&
		global->c_ray.hit.point.y == debug_y);

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
        
        // MODIFICAR: manejo especial para transparencia
        if (depth == 0)
        {
            // Guardamos el color de superficie
            surface_color = level_contribution;
            accumulated_color = level_contribution;
            
            // Verificamos si hay transparencia
            if (global->c_ray.hit.obj_type >= 0)
            {
                transparency = get_object_transparency(global, global->c_ray.hit);
                if (transparency > 0.1f)
                    found_transparency = 1;
            }
        }
        else if (found_transparency && depth == 1)
        {
            // Aumentar brillo de objetos vistos a través de transparencia
            level_contribution.r = (int)(level_contribution.r * 1.3f);
            level_contribution.g = (int)(level_contribution.g * 1.3f);
            level_contribution.b = (int)(level_contribution.b * 1.3f);
            
            // Mezclar con la mejora
            accumulated_color.r = (int)(surface_color.r * (1.0f - transparency) + 
                    level_contribution.r * transparency);
            accumulated_color.g = (int)(surface_color.g * (1.0f - transparency) + 
                    level_contribution.g * transparency);
            accumulated_color.b = (int)(surface_color.b * (1.0f - transparency) + 
                    level_contribution.b * transparency);
        }
        else
        {
            // Para reflejos y otros efectos, seguimos usando la lógica actual
            // Pero con un peso reducido para no saturar
            float blend_factor = 0.3f;
            accumulated_color.r = (int)(accumulated_color.r * (1.0f - blend_factor) + 
                            level_contribution.r * blend_factor);
            accumulated_color.g = (int)(accumulated_color.g * (1.0f - blend_factor) + 
                            level_contribution.g * blend_factor);
            accumulated_color.b = (int)(accumulated_color.b * (1.0f - blend_factor) + 
                            level_contribution.b * blend_factor);
        }

        printf("Depth %d: %d rays, Accumulated color: (%d,%d,%d)\n", 
            depth, current_count, accumulated_color.r, 
            accumulated_color.g, accumulated_color.b);
            
        // Only generate secondary rays if we have more depth to go
        if (depth < max_depth - 1)
        {
            int i = 0;
            next_count = 0; // CORREGIDO: Inicializar antes del bucle
            while (i < current_count)
            {
                // Guarda el estado actual
                t_ray_state saved_state = save_ray_state(global);
                
                // Configura el rayo actual
                global->c_ray.origin = current_level[i].origin;
                global->c_ray.dir = current_level[i].direction;
                global->c_ray.hit = find_closest_isec(global);
                
                // Solo genera rayos secundarios si hay una intersección
                if (global->c_ray.hit.obj_type >= 0)
                {
                    generate_secondary_rays(global, next_level, &next_count);
                    if(debug_this_ray)
                        printf("Secondary rays for next level: %d\n", next_count);
                }
                
                // Restaura el estado antes de procesar el siguiente rayo
                restore_ray_state(global, saved_state);
                i++;
            }
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
    printf("FINAL COLOR: (%d,%d,%d)\n\n", 
       accumulated_color.r, accumulated_color.g, accumulated_color.b);
    return (accumulated_color);
}