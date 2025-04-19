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

// Guardar el estado actual del ray tracing
t_ray_state	save_ray_state(t_global *global)
{
	t_ray_state	state;

	state.origin = global->current_ray_origin;
	state.dir = global->current_ray_dir;
	state.intersec = global->current_intersec;
	return (state);
}

// Restaurar el estado del ray tracing
void	restore_ray_state(t_global *global, t_ray_state state)
{
	global->current_ray_origin = state.origin;
	global->current_ray_dir = state.dir;
	global->current_intersec = state.intersec;
}

// Preparar datos para cálculos de iluminación
void	prepare_lighting_data(t_global *global)
{
	// Calcular datos que se usarán en varias funciones
	global->current_object_color = get_object_color(global);
	global->current_normal = get_surface_normal(global,
			global->current_intersec);
	global->current_light_dir = normalize(subtract(global->scene.light.pos,
				global->current_intersec.point));
	global->current_light_distance = magnitude(subtract(global->scene.light.pos,
				global->current_intersec.point));
}

// Calcular si un punto está en sombra
float	cal_shadow(t_global *global)
{
	float		light_intensity;
	t_vector	shadow_origin;
	t_ray_state	state;
	t_intersec	shadow_intersec;

	light_intensity = global->scene.light.intensity;
	// Desplazar ligeramente el origen para evitar auto-intersecciones
	shadow_origin = add(global->current_intersec.point,
			multiply(global->current_normal, 0.005f));
	// Guardar estado actual
	state = save_ray_state(global);
	// Configurar rayo de sombra directamente
	global->current_ray_origin = shadow_origin;
	global->current_ray_dir = global->current_light_dir;
	shadow_intersec = find_closest_intersec(global);
	// Restaurar estado original
	restore_ray_state(global, state);
	// Verificar si hay sombra
	if (shadow_intersec.obj_type >= 0
		&& shadow_intersec.dist < global->current_light_distance
		&& !(shadow_intersec.obj_type == global->current_intersec.obj_type
			&& shadow_intersec.obj_index == global->current_intersec.obj_index))
	{
		return (0); // Punto en sombra
	}
	return (light_intensity);
}

// Calcular componente ambiental
t_color	calculate_ambient(t_color object_color, float ambient_intensity)
{
	t_color	result;

	result.r = object_color.r * ambient_intensity;
	result.g = object_color.g * ambient_intensity;
	result.b = object_color.b * ambient_intensity;
	return (result);
}

// Calcular componente difusa (versión simplificada)
void	add_diffuse(t_global *global, t_color *color, float light_intensity)
{
	float	diff;

	diff = fmax(0.0f, dot(global->current_normal, global->current_light_dir));
	color->r += global->current_object_color.r * light_intensity * diff;
	color->g += global->current_object_color.g * light_intensity * diff;
	color->b += global->current_object_color.b * light_intensity * diff;
}

// Calcular componente especular (versión simplificada)
void	add_specular(t_global *global, t_color *color, float light_intensity)
{
	t_vector	reflect_dir;
	float		spec;

	reflect_dir = subtract(multiply(global->current_normal, 2.0f
				* dot(global->current_normal, global->current_light_dir)),
			global->current_light_dir);
	spec = pow(fmax(0.0f, dot(normalize(multiply(global->current_ray_dir,
							-1.0f)), reflect_dir)), 32);
	color->r += 255 * spec * light_intensity * 0.5f;
	color->g += 255 * spec * light_intensity * 0.5f;
	color->b += 255 * spec * light_intensity * 0.5f;
}

// Limitar valores de color al rango [0, 255]
void	clamp_color(t_color *color)
{
	color->r = fmin(255, fmax(0, color->r));
	color->g = fmin(255, fmax(0, color->g));
	color->b = fmin(255, fmax(0, color->b));
}

// Función principal de aplicación de iluminación
t_color	apply_lighting(t_global *global, float light_intensity)
{
	t_color	result_color;

	// Calcular iluminación ambiental (siempre presente)
	result_color = calculate_ambient(global->current_object_color,
			global->scene.ambient.intensity);
	// Añadir componentes difusa y especular si no está en sombra
	if (light_intensity > 0)
	{
		add_diffuse(global, &result_color, light_intensity);
		add_specular(global, &result_color, light_intensity);
	}
	// Limitar valores finales
	clamp_color(&result_color);
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
	// Preparar datos para cálculos de iluminación
	prepare_lighting_data(global);
	// Calcular sombras e iluminación
	light_intensity = cal_shadow(global);
	// Aplicar modelo de iluminación
	return (apply_lighting(global, light_intensity));
}
