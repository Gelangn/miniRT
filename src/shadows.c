// Incluir encabezado 42
#include "minirt.h"

float	cal_shadow(t_global *global)
{
	t_vector shadow_origin;
	t_ray_state state;
	t_intersec shadow_intersec;

	shadow_origin = add(global->current_intersec.point,
			multiply(global->current_normal, 0.005f));
	state = save_ray_state(global);
	global->current_ray_origin = shadow_origin;
	global->current_ray_dir = global->current_light_dir;
	shadow_intersec = find_closest_intersec(global);
	restore_ray_state(global, state);
	if (shadow_intersec.obj_type >= 0
		&& shadow_intersec.dist < global->current_light_distance
		&& !(shadow_intersec.obj_type == global->current_intersec.obj_type
			&& shadow_intersec.obj_index == global->current_intersec.obj_index))
	{
		return (0.0f); // En sombra
	}
	return (1.0f); // No en sombra
}