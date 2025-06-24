/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_inside_test.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:00:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/25 01:07:37 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Tests if a point is inside a sphere
 */
int is_inside_sphere(t_sphere *sphere, t_vector point)
{
    float distance;
    
    distance = mag(subtract(point, sphere->center));
    return (distance < sphere->radius);
}

/**
 * Tests if a point is inside a cylinder
 */
int is_inside_cylinder(t_cylinder *cyl, t_vector point)
{
    t_vector to_point;
    t_vector axis_dir;
    float dist_to_axis;
    float proj_length;
    
    // Normalizar el eje para asegurar cálculos correctos
    axis_dir = norm(cyl->axis);
    
    // Vector desde el punto base al punto
    to_point = subtract(point, cyl->base);
    
    // Componente de to_point en la dirección del eje
    proj_length = dot(to_point, axis_dir);
    
    // La proyección vectorial sobre el eje
    t_vector proj_on_axis = multiply(axis_dir, proj_length);
    
    // La distancia perpendicular al eje
    dist_to_axis = mag(subtract(to_point, proj_on_axis));
    
    // Verificar si está dentro del cilindro:
    // 1. Distancia al eje < radio
    // 2. Proyección sobre el eje está entre 0 y altura
    return (dist_to_axis < cyl->radius && 
            proj_length >= 0 && 
            proj_length <= cyl->height);
}

/* *
 * Tests if ray origin is inside the object at the intersection
 * Currently only works for spheres
 */
int is_inside_object(t_global *global, t_intersec isec, t_vector origin)
{
    // Esferas
    if (isec.obj_type == 0) {
        return is_inside_sphere(&global->scene.spheres[isec.obj_index], origin);
    }
    // Cilindros
    else if (isec.obj_type == 2) {
		//printf("is_inside cylinder\n");
		return is_inside_cylinder(&global->scene.cyls[isec.obj_index], origin);
	}
    
    return (0); // Para planos siempre falso (no tienen "dentro")
}
