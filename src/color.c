// incluir cabecera 42

#include "../inc/minirt.h"

int	rgb_to_int(t_color color)
{
	return ((color.r << 16) | (color.g << 8) | color.b);
}

t_color	cal_lighting(t_global *global)
{
	t_intersec	intersec;
	t_vector	ray_dir;
	t_color		result_color;
	t_vector	normal;
	t_vector	light_dir;
	t_color		object_color;
	float		light_intensity;
	t_vector	shadow_origin;
	t_vector	saved_origin;
	t_vector	saved_dir;
	t_intersec	saved_intersec;
	t_intersec	shadow_intersec;
	float		light_distance;
	float		diff;
	t_vector	reflect_dir;
	float		spec;

	intersec = global->current_intersec;
	ray_dir = global->current_ray_dir;
	// Verificación inicial para evitar acceder a objetos inexistentes
	if ((intersec.obj_type < 0) || (intersec.obj_index < 0)
		|| (intersec.obj_type == 0
			&& intersec.obj_index >= global->scene.num_sp)
		|| (intersec.obj_type == 1
			&& intersec.obj_index >= global->scene.num_pl)
		|| (intersec.obj_type == 2
			&& intersec.obj_index >= global->scene.num_cy))
	{
		// Devolver un color por defecto (negro o gris oscuro)
		t_color default_color = {5, 5, 5}; // Gris muy oscuro
		return (default_color);
	}
	normal = get_surface_normal(global, intersec);
	light_dir = normalize(subtract(global->scene.light.pos, intersec.point));
	light_intensity = global->scene.light.intensity;
	// Obtener color del objeto
	if (intersec.obj_type == 0)
		object_color = global->scene.spheres[intersec.obj_index].color;
	else if (intersec.obj_type == 1)
		object_color = global->scene.planes[intersec.obj_index].color;
	else
		object_color = global->scene.cylinders[intersec.obj_index].color;
	// Comprobar sombras
	shadow_origin = add(intersec.point, multiply(normal, 0.005f));
	// Guarda los valores actuales del rayo
	saved_origin = global->current_ray_origin;
	saved_dir = global->current_ray_dir;
	saved_intersec = global->current_intersec;
	// Configura el rayo de sombra
	global->current_ray_origin = shadow_origin;
	global->current_ray_dir = light_dir;
	// Llama a find_closest_intersec
	shadow_intersec = find_closest_intersec(global);
	// Restaura los valores originales
	global->current_ray_origin = saved_origin;
	global->current_ray_dir = saved_dir;
	global->current_intersec = saved_intersec;
	light_distance = magnitude(subtract(global->scene.light.pos,
				intersec.point));
	// Si hay una intersección antes de llegar a la luz, el punto está en sombra
	if (shadow_intersec.obj_type >= 0 && shadow_intersec.dist < light_distance
		&& !(shadow_intersec.obj_type == intersec.obj_type
			&& shadow_intersec.obj_index == intersec.obj_index))
	{
		light_intensity = 0; // Solo luz ambiental
	}
	// Componente ambiental (siempre presente)
	result_color.r = object_color.r * global->scene.ambient.intensity;
	result_color.g = object_color.g * global->scene.ambient.intensity;
	result_color.b = object_color.b * global->scene.ambient.intensity;
	// Añadir componentes difusa y especular solo si no está en sombra
	if (light_intensity > 0)
	{
		// Componente difusa
		diff = fmax(0.0f, dot(normal, light_dir));
		result_color.r += object_color.r * light_intensity * diff;
		result_color.g += object_color.g * light_intensity * diff;
		result_color.b += object_color.b * light_intensity * diff;
		// Componente especular
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

t_color	cal_color(t_global *global, t_intersec intersec)
{
	// Si no hay intersección, devolver color de fondo
	if (intersec.obj_type < 0)
	{
		t_color bg_color = {5, 5, 5}; // Gris oscuro de fondo
		return (bg_color);
	}

	// Guardar la intersección actual en global
	global->current_intersec = intersec;

	// Calcular el color con iluminación
	return (cal_lighting(global));
}