/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_inside_test.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:00:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/20 00:40:53 by anavas-g         ###   ########.fr       */
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

/* *
 * Tests if ray origin is inside the object at the intersection
 * Currently only works for spheres
 */
int is_inside_object(t_global *global, t_intersec isec, t_vector origin)
{
    // Only spheres can be "inside" for now
    if (isec.obj_type == 0) {
        return is_inside_sphere(&global->scene.spheres[isec.obj_index], origin);
    }
    
    return (0);
}
