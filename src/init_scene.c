/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:39:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/05 21:25:37 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	init_scene(t_global *global)
{
	t_scene	*scene;

	scene = &global->scene;
	scene->file_path = NULL;
	scene->fd = 0;
	scene->lines = NULL;
	scene->scale = 0;
	scene->ambient = (t_ambient){0};
	scene->cam = (t_camera){0};
	scene->light = (t_light){0};
	scene->ambient.init = 0;
	scene->cam.init = 0;
	scene->light.init = 0;
	scene->spheres = NULL;
	scene->planes = NULL;
	scene->cyls = NULL;
	scene->num_sp = 0;
	scene->num_pl = 0;
	scene->num_cy = 0;
}

t_intersec	init_intersec(void)
{
	t_intersec	isec;

	isec.dist = INFINITY;
	isec.point = (t_vector){0, 0, 0};
	isec.obj_index = -1;
	isec.obj_type = -1;
	return (isec);
}

void	init_lateral_isec_vars(t_global *global, int cy_id)
{
	t_cylinder	*cyl;

	cyl = &global->scene.cyls[cy_id];
	global->current_cyl_vars.axis = norm(cyl->orientation);
	global->current_cyl_vars.oc = subtract(global->c_ray.origin, cyl->base);
	global->current_cyl_vars.dir_dot_axis = dot(global->c_ray.dir,
			global->current_cyl_vars.axis);
	global->current_cyl_vars.oc_dot_axis = dot(global->current_cyl_vars.oc,
			global->current_cyl_vars.axis);
	global->current_cyl_vars.dir_perp = subtract(global->c_ray.dir,
			multiply(global->current_cyl_vars.axis,
				global->current_cyl_vars.dir_dot_axis));
	global->current_cyl_vars.oc_perp = subtract(global->current_cyl_vars.oc,
			multiply(global->current_cyl_vars.axis,
				global->current_cyl_vars.oc_dot_axis));
}

void	check_scene(t_global *global, t_scene *scene)
{
	if (scene->ambient.init == 0 || scene->cam.init == 0
		|| scene->light.init == 0)
		finish(global, ERR_SCENE);
	if (scene->num_sp > MAX_SPHERES)
		finish(global, ERR_SPHERE);
	if (scene->num_pl > MAX_PLANES)
		finish(global, ERR_PLANE);
	if (scene->num_cy > MAX_CYLINDERS)
		finish(global, ERR_CYLINDER);
	if (scene->num_sp == 0 && scene->num_pl == 0 && scene->num_cy == 0)
		finish(global, ERR_SCENE);
}
