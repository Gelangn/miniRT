// incluir cabecera 42

#include "../inc/minirt.h"

int	rgb_to_int(t_color color)
{
	return ((color.r << 16) | (color.g << 8) | color.b);
}

// Obtener el color del objeto
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

// Calcular sombras
float	cal_shadow(t_global *global)
{
	t_intersec	intersec;
	t_vector	normal;
	t_vector	light_dir;
	float		light_intensity;
	t_vector	shadow_origin;
	t_vector	saved_origin;
	t_vector	saved_dir;
	t_intersec	saved_intersec;
	t_intersec	shadow_intersec;
	float		light_distance;

	intersec = global->current_intersec;
	normal = get_surface_normal(global, intersec);
	light_dir = normalize(subtract(global->scene.light.pos, intersec.point));
	light_intensity = global->scene.light.intensity;
	// Punto de origen ligeramente desplazado
	shadow_origin = add(intersec.point, multiply(normal, 0.005f));
	// Guardar estado actual
	saved_origin = global->current_ray_origin;
	saved_dir = global->current_ray_dir;
	saved_intersec = global->current_intersec;
	// Configurar rayo de sombra
	global->current_ray_origin = shadow_origin;
	global->current_ray_dir = light_dir;
	shadow_intersec = find_closest_intersec(global);
	// Restaurar estado
	global->current_ray_origin = saved_origin;
	global->current_ray_dir = saved_dir;
	global->current_intersec = saved_intersec;
	light_distance = magnitude(subtract(global->scene.light.pos,
				intersec.point));
	// Verificar si hay sombra
	if (shadow_intersec.obj_type >= 0 && shadow_intersec.dist < light_distance
		&& !(shadow_intersec.obj_type == intersec.obj_type
			&& shadow_intersec.obj_index == intersec.obj_index))
	{
		light_intensity = 0;
	}
	return (light_intensity);
}

// Aplicar componentes de iluminación
t_color	apply_lighting(t_global *global, float light_intensity)
{
	t_color		object_color;
	t_intersec	intersec;
	t_vector	ray_dir;
	t_vector	normal;
	t_vector	light_dir;
	float		ambient_intensity;
	t_color		result_color;
	float		diff;
	t_vector	reflect_dir;
	float		spec;

	object_color = get_object_color(global);
	intersec = global->current_intersec;
	ray_dir = global->current_ray_dir;
	normal = get_surface_normal(global, intersec);
	light_dir = normalize(subtract(global->scene.light.pos, intersec.point));
	ambient_intensity = global->scene.ambient.intensity;
	// Componente ambiental
	result_color.r = object_color.r * ambient_intensity;
	result_color.g = object_color.g * ambient_intensity;
	result_color.b = object_color.b * ambient_intensity;
	// Componentes difusa y especular
	if (light_intensity > 0)
	{
		// Difusa
		diff = fmax(0.0f, dot(normal, light_dir));
		result_color.r += object_color.r * light_intensity * diff;
		result_color.g += object_color.g * light_intensity * diff;
		result_color.b += object_color.b * light_intensity * diff;
		// Especular
		reflect_dir = subtract(multiply(normal, 2.0f * dot(normal, light_dir)),
				light_dir);
		spec = pow(fmax(0.0f, dot(normalize(multiply(ray_dir, -1.0f)),
						reflect_dir)), 32);
		result_color.r += 255 * spec * 0.5f;
		result_color.g += 255 * spec * 0.5f;
		result_color.b += 255 * spec * 0.5f;
	}
	// Limitar valores
	result_color.r = fmin(255, fmax(0, result_color.r));
	result_color.g = fmin(255, fmax(0, result_color.g));
	result_color.b = fmin(255, fmax(0, result_color.b));
	return (result_color);
}

// Verificar si la intersección es válida
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

// Función principal simplificada
t_color	cal_lighting(t_global *global)
{
	float	light_intensity;

	// Verificar intersección válida
	if (!is_valid_intersec(global))
	{
		t_color default_color = {5, 5, 5}; // Gris oscuro
		return (default_color);
	}
	// Calcular sombras e iluminación
	light_intensity = cal_shadow(global);
	// Aplicar modelo de iluminación
	return (apply_lighting(global, light_intensity));
}
