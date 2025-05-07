/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_intersection.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 00:00:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/07 20:14:17 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Adaptación para usar las funciones existentes de cálculo de intersecciones
t_intersec	check_object_intersection(t_ray ray, t_object *obj,
		t_global *global)
{
	t_intersec	isec;
	t_ray_state	original_state;

	// Inicialización correcta de la estructura
	isec.dist = INFINITY;
	isec.point = (t_vector){0, 0, 0};
	isec.obj_index = -1;
	isec.obj_type = -1;
	// Guardar el rayo actual para restaurarlo después
	original_state = save_ray_state(global);
	// Configurar el rayo actual en la estructura global
	global->c_ray.origin = ray.origin;
	global->c_ray.dir = ray.dir;
	if (obj->type == OBJ_SPHERE)
	{
		// Usar col_sp para esferas
		isec = col_sp(global, obj - global->objects);
	}
	else if (obj->type == OBJ_PLANE)
	{
		// Usar col_pl para planos
		isec = col_pl(global, obj - global->objects);
	}
	else if (obj->type == OBJ_CYLINDER)
	{
		// Usar col_cy para cilindros
		isec = col_cy(global, obj - global->objects);
	}
	// Restaurar el rayo original
	restore_ray_state(global, original_state);
	return (isec);
}

// Corregir la función find_intersection_bvh
t_intersec	find_intersection_bvh(t_ray ray, t_bvh_node *node, t_global *global)
{
	t_intersec	closest;
	int			i;
	t_intersec	isec;
	t_intersec	left_isec;
	t_intersec	right_isec;

	// Inicialización correcta
	closest.dist = INFINITY;
	closest.point = (t_vector){0, 0, 0};
	closest.obj_index = -1;
	closest.obj_type = -1;
	// Verificar si el rayo intersecta con este nodo
	if (!ray_intersects_aabb(ray.origin, ray.dir, node->bounds))
		return (closest);
	// Si es un nodo hoja, verificar todos los objetos
	if (node->objects)
	{
		i = 0;
		while (i < node->object_count)
		{
			isec = check_object_intersection(ray, node->objects[i], global);
			if (isec.dist < closest.dist)
				closest = isec;
			i++;
		}
		return (closest);
	}
	// Para nodos internos, verificar ambos hijos
	left_isec = find_intersection_bvh(ray, node->left, global);
	right_isec = find_intersection_bvh(ray, node->right, global);
	// Retornar la intersección más cercana
	if (left_isec.dist < right_isec.dist)
		return (left_isec);
	else
		return (right_isec);
}

// Función principal de intersección usando BVH
t_intersec	find_closest_intersec_bvh(t_global *global)
{
	static int hits = 0;
	static int misses = 0;
	t_intersec result;
	t_ray ray;

	// Inicialización correcta
	ray.origin = global->c_ray.origin;
	ray.dir = global->c_ray.dir;

	// Código existente...
	result = find_intersection_bvh(ray, global->bvh_root, global);

	// Diagnóstico de intersecciones
	if (result.dist < INFINITY && result.obj_type >= 0)
		hits++;
	else
		misses++;

	if ((hits + misses) % 10000 == 0)
		printf("BVH stats: Hits: %d, Misses: %d (%.2f%%)\n",
				hits,
				misses,
				100.0f * hits / (hits + misses));

	return (result);
}