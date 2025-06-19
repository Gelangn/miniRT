/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_intersecs.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:59:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 11:55:16 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Finds the closest intersection point for the current ray with any object
 * Tests intersections with all object types (spheres, planes, cylinders)
 * Returns the intersection data for the closest hit point
 * 
 * @param global Structure containing scene data and current ray information
 * @return Intersection data for the closest object hit by the ray
 */
t_intersec	find_closest_isec(t_global *global)
{
	t_intersec	closest_isec;

	closest_isec = init_isec();
	if (global->scene.num_sp > 0)
		check_obj_isecs(global, &closest_isec, 0);
	if (global->scene.num_pl > 0)
		check_obj_isecs(global, &closest_isec, 1);
	if (global->scene.num_cy > 0)
		check_obj_isecs(global, &closest_isec, 2);
	return (closest_isec);
}

/**
 * Validates if the current intersection data is valid
 * Checks object type, index bounds, and ensures intersection exists
 * Used to prevent processing invalid or non-existent intersections
 * 
 * @param global Structure containing scene data and intersection information
 * @return 1 if intersection is valid, 0 otherwise
 */
int	is_valid_isec(t_global *global)
{
	t_intersec	isec;

	isec = global->c_ray.hit;
	if ((isec.obj_type < 0) || (isec.obj_index < 0) || (isec.obj_type == 0
			&& isec.obj_index >= global->scene.num_sp) || (isec.obj_type == 1
			&& isec.obj_index >= global->scene.num_pl) || (isec.obj_type == 2
			&& isec.obj_index >= global->scene.num_cy))
	{
		return (0);
	}
	return (1);
}

/**
 * Returns the maximum number of objects for a given object type
 * Helper function to get array bounds for intersection testing
 * 
 * @param global Structure containing scene data with object counts
 * @param obj_type Type of object (0=sphere, 1=plane, 2=cylinder)
 * @return Number of objects of the specified type in the scene
 */
static int	get_max_objects(t_global *global, int obj_type)
{
	if (obj_type == 0)
		return (global->scene.num_sp);
	else if (obj_type == 1)
		return (global->scene.num_pl);
	else if (obj_type == 2)
		return (global->scene.num_cy);
	return (0);
}

/**
 * Tests intersections with all objects of a specific type
 * Iterates through all objects of the given type and finds the closest hit
 * Updates the closest intersection if a nearer intersection is found
 * 
 * @param global Structure containing scene data and current ray
 * @param closest Pointer to current closest intersection data to update
 * @param obj_type Type of objects to test (0=sphere, 1=plane, 2=cylinder)
 */
void	check_obj_isecs(t_global *global, t_intersec *closest, int obj_type)
{
	int			i;
	int			max_objs;
	t_intersec	temp;

	max_objs = get_max_objects(global, obj_type);
	i = -1;
	while (++i < max_objs)
	{
		if (obj_type == 0)
			temp = col_sp(global, i);
		else if (obj_type == 1)
			temp = col_pl(global, i);
		else
			temp = col_cy(global, i);
		if (temp.dist < closest->dist)
		{
			*closest = temp;
			closest->obj_index = i;
			closest->obj_type = obj_type;
		}
	}
}
