/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracer_properties.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-mada <bde-mada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 18:36:23 by bde-mada          #+#    #+#             */
/*   Updated: 2025/06/18 18:40:28 by bde-mada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

float	get_object_transparency(t_global *global, t_intersec isec)
{
    if (isec.obj_type == 0 && isec.obj_index < global->scene.num_sp)
        return (global->scene.spheres[isec.obj_index].transparency);
    else if (isec.obj_type == 1 && isec.obj_index < global->scene.num_pl)
        return (global->scene.planes[isec.obj_index].transparency);
    else if (isec.obj_type == 2 && isec.obj_index < global->scene.num_cy)
        return (global->scene.cyls[isec.obj_index].transparency);
    return (0.0f);
}

float	get_object_reflectivity(t_global *global, t_intersec isec)
{
    if (isec.obj_type == 0 && isec.obj_index < global->scene.num_sp)
        return (global->scene.spheres[isec.obj_index].reflectivity);
    else if (isec.obj_type == 1 && isec.obj_index < global->scene.num_pl)
        return (global->scene.planes[isec.obj_index].reflectivity);
    else if (isec.obj_type == 2 && isec.obj_index < global->scene.num_cy)
        return (global->scene.cyls[isec.obj_index].reflectivity);
    return (0.0f);
}

float	get_object_refractive_index(t_global *global, t_intersec isec)
{
    if (isec.obj_type == 0 && isec.obj_index < global->scene.num_sp)
        return (global->scene.spheres[isec.obj_index].refractive_index);
    else if (isec.obj_type == 1 && isec.obj_index < global->scene.num_pl)
        return (global->scene.planes[isec.obj_index].refractive_index);
    else if (isec.obj_type == 2 && isec.obj_index < global->scene.num_cy)
        return (global->scene.cyls[isec.obj_index].refractive_index);
    return (AIR_REFRACTIVE_INDEX);
}