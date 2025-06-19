/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_normals.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 20:54:37 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 11:54:02 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Calculates the surface normal vector for a sphere at a given point

	* The normal is computed as the normalized vector from sphere center
	to the point
 * 
 * @param global Structure containing scene data including spheres array
 * @param sp_id Index of the sphere in the spheres array
 * @param point Point on the sphere surface where normal is calculated
 * @return Normalized vector representing the surface normal at the given point
 */
t_vector	get_sp_normal(t_global *global, int sp_id, t_vector point)
{
	t_sphere	sphere;

	sphere = global->scene.spheres[sp_id];
	return (norm(subtract(point, sphere.center)));
}

/**
 * Returns the surface normal vector for a plane
 * Performs bounds checking and returns the plane's normalized normal vector
 * 
 * @param global Structure containing scene data including planes array
 * @param pl_id Index of the plane in the planes array
 * @return Normalized normal vector of the plane,
	or default (0,1,0) if invalid index
 */
t_vector	get_pl_normal(t_global *global, int pl_id)
{
	if (pl_id < 0 || pl_id >= global->scene.num_pl)
		return ((t_vector){0, 1, 0});
	return (norm(global->scene.planes[pl_id].normal));
}

/**
 * Calculates the surface normal vector for a cylinder at the intersection point
 * Determines if the intersection is on the top cap, bottom cap,
	or lateral surface
 * Returns appropriate normal vector based on the intersection location
 * 
 * @param global Structure containing scene data including cylinders array
 * @param isec Intersection data containing point and cylinder index

	* @return Normalized vector representing the surface normal at
	intersection point
 */
t_vector	get_cy_normal(t_global *global, t_intersec isec)
{
	t_cylinder	cyl;
	t_vector	axis;
	float		hit_h;
	t_vector	ctr_h;

	cyl = global->scene.cyls[isec.obj_index];
	axis = norm(cyl.orientation);
	hit_h = dot(subtract(isec.point, cyl.base), axis);
	if (comp_floats(hit_h, 0))
		return (multiply(axis, -1));
	else if (comp_floats(hit_h, cyl.height))
		return (axis);
	else
	{
		ctr_h = add(cyl.base, multiply(axis, hit_h));
		return (norm(subtract(isec.point, ctr_h)));
	}
}

/**
 * Determines and returns the appropriate surface normal for any object type
 * Dispatches to specific normal calculation functions based on object type
 * Performs validation checks for object type and index bounds
 * 
 * @param global Structure containing all scene data
 * @param isec Intersection data containing object type, index, and point
 * @return Normalized surface normal vector for the intersected object
 */
t_vector	get_surface_normal(t_global *global, t_intersec isec)
{
	int	obj_type;
	int	obj_index;

	obj_type = isec.obj_type;
	obj_index = isec.obj_index;
	if (obj_type < 0 || obj_index < 0 || (obj_type == 0
			&& obj_index >= global->scene.num_sp) || (obj_type == 1
			&& obj_index >= global->scene.num_pl) || (obj_type == 2
			&& obj_index >= global->scene.num_cy))
		return ((t_vector){0, 1, 0});
	if (obj_type == 0)
		return (get_sp_normal(global, obj_index, isec.point));
	else if (obj_type == 1)
		return (get_pl_normal(global, obj_index));
	else
		return (get_cy_normal(global, isec));
}
