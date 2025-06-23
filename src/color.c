/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:13:10 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/23 11:13:35 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Scales a color by a given factor
 * Multiplies each RGB component by the specified scaling factor
 * Used for dimming/brightening colors in lighting calculations
 * 
 * @param color Original color to be scaled
 * @param factor Scaling factor to apply to each RGB component
 * @return New color with scaled RGB values
 */
t_color	color_scale(t_color color, float factor)
{
	t_color	result;

	result.r = color.r * factor;
	result.g = color.g * factor;
	result.b = color.b * factor;
	return (result);
}

/**
 * Adds two colors together by combining their RGB components
 * Performs component-wise addition for color blending operations
 * Used to combine ambient, diffuse, and specular lighting contributions
 * 
 * @param c1 First color to add
 * @param c2 Second color to add
 * @return New color with combined RGB values
 */
t_color	color_add(t_color c1, t_color c2)
{
	t_color	result;

	result.r = c1.r + c2.r;
	result.g = c1.g + c2.g;
	result.b = c1.b + c2.b;
	return (result);
}

/**
 * Retrieves the base color of the intersected object
 * Determines object type from intersection data and returns corresponding color
 * Supports spheres, planes, and cylinders
 * 
 * @param global Structure containing scene data and current ray intersection
 * @return The RGB color of the intersected object
 */
t_color	get_object_color(t_global *global)
{
	if (!is_valid_isec(global))
		return ((t_color){0, 0, 0});
	if (global->c_ray.hit.obj_type == 0)
		return (global->scene.spheres[global->c_ray.hit.obj_index].color);
	else if (global->c_ray.hit.obj_type == 1)
		return (global->scene.planes[global->c_ray.hit.obj_index].color);
	else if (global->c_ray.hit.obj_type == 2)
		return (global->scene.cyls[global->c_ray.hit.obj_index].color);
	return ((t_color){0, 0, 0});
}

/**
 * Saves the current ray state for later restoration
 * Preserves ray origin, direction, and intersection data
 * Used for shadow ray calculations and reflection operations
 * 
 * @param global Structure containing current ray information
 * @return Ray state structure with saved data
 */
t_ray_state	save_ray_state(t_global *global)
{
	global->state.origin = global->c_ray.origin;
	global->state.dir = global->c_ray.dir;
	global->state.isec = global->c_ray.hit;
	return (global->state);
}

/**
 * Restores a previously saved ray state
 * Recovers ray origin, direction, and intersection data
 * Used after shadow ray or reflection calculations to restore original ray
 * 
 * @param global Structure to restore ray information to
 * @param state Previously saved ray state to restore
 */
void	restore_ray_state(t_global *global)
{
	global->c_ray.origin = global->state.origin;
	global->c_ray.dir = global->state.dir;
	global->c_ray.hit = global->state.isec;
}
