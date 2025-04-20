// incluir cabecera 42

#include "../inc/minirt.h"

int	rgb_to_int(t_color color)
{
	return ((color.r << 16) | (color.g << 8) | color.b);
}

t_color	color_scale(t_color color, float factor)
{
	t_color	result;

	result.r = color.r * factor;
	result.g = color.g * factor;
	result.b = color.b * factor;
	return (result);
}

t_color	color_add(t_color c1, t_color c2)
{
	t_color	result;

	result.r = c1.r + c2.r;
	result.g = c1.g + c2.g;
	result.b = c1.b + c2.b;
	return (result);
}

t_color	get_object_color(t_global *global)
{
	t_intersec	intersec;

	intersec = global->current_intersec;
	if (intersec.obj_type == 0)
		return (global->scene.spheres[intersec.obj_index].color);
	else if (intersec.obj_type == 1)
		return (global->scene.planes[intersec.obj_index].color);
	else
		return (global->scene.cylinders[intersec.obj_index].color);
}

t_ray_state	save_ray_state(t_global *global)
{
	t_ray_state	state;

	state.origin = global->current_ray_origin;
	state.dir = global->current_ray_dir;
	state.intersec = global->current_intersec;
	return (state);
}

void	restore_ray_state(t_global *global, t_ray_state state)
{
	global->current_ray_origin = state.origin;
	global->current_ray_dir = state.dir;
	global->current_intersec = state.intersec;
}

t_color	calculate_ambient(t_global *global)
{
	t_color	result;

	result.r = global->current_object_color.r * global->scene.ambient.intensity;
	result.g = global->current_object_color.g * global->scene.ambient.intensity;
	result.b = global->current_object_color.b * global->scene.ambient.intensity;
	return (result);
}

t_color	calculate_diffuse(t_global *global)
{
	float	diff;
	float	light_intensity;

	light_intensity = global->scene.light.intensity;
	diff = fmax(0.0f, dot(global->current_normal, global->current_light_dir));
	return (color_scale(global->current_object_color, light_intensity * diff));
}

t_color	calculate_specular(t_global *global)
{
	t_vector	view_dir;
	t_vector	reflect_dir;
	float		spec;
	t_color		white;
	float		light_intensity;

	light_intensity = global->scene.light.intensity;
	view_dir = normalize(multiply(global->current_ray_dir, -1.0f));
	reflect_dir = subtract(multiply(global->current_normal, 2.0f
				* dot(global->current_normal, global->current_light_dir)),
			global->current_light_dir);
	spec = pow(fmax(0.0f, dot(view_dir, reflect_dir)), 32);
	white.r = 255;
	white.g = 255;
	white.b = 255;
	return (color_scale(white, spec * light_intensity * 0.5f));
}

void	clamp_color(t_color *color)
{
	color->r = fmin(255, fmax(0, color->r));
	color->g = fmin(255, fmax(0, color->g));
	color->b = fmin(255, fmax(0, color->b));
}

int	is_valid_intersec(t_global *global)
{
	t_intersec	intersec;

	intersec = global->current_intersec;
	if ((intersec.obj_type < 0) || (intersec.obj_index < 0)
		|| (intersec.obj_type == 0
			&& intersec.obj_index >= global->scene.num_sp)
		|| (intersec.obj_type == 1
			&& intersec.obj_index >= global->scene.num_pl)
		|| (intersec.obj_type == 2
			&& intersec.obj_index >= global->scene.num_cy))
	{
		return (0);
	}
	return (1);
}
