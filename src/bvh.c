/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 00:00:00 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/07 20:15:09 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Crea un AABB para un objeto específico
t_aabb	create_aabb_for_object(t_object *obj)
{
	t_aabb		bounds;
	t_sphere	*sphere;
	t_vector	radius_vec;

	if (obj->type == OBJ_SPHERE)
	{
		sphere = &obj->data.sphere;
		radius_vec = (t_vector){sphere->radius, sphere->radius, sphere->radius};
		bounds.min = subtract(sphere->center, radius_vec);
		bounds.max = add(sphere->center, radius_vec);
	}
	else if (obj->type == OBJ_CYLINDER)
	{
		// Implementar cálculo de AABB para cilindros
		// Esto es más complejo y requiere calcular puntos extremos
	}
	else if (obj->type == OBJ_PLANE)
	{
		// Los planos son infinitos, así que esto es una aproximación
		// Puede ser mejorado para la escena específica
		bounds.min = (t_vector){-INFINITY, -INFINITY, -INFINITY};
		bounds.max = (t_vector){INFINITY, INFINITY, INFINITY};
	}
	return (bounds);
}

// Combina dos AABBs en uno
t_aabb	combine_aabb(t_aabb a, t_aabb b)
{
	t_aabb	result;

	result.min.x = fminf(a.min.x, b.min.x);
	result.min.y = fminf(a.min.y, b.min.y);
	result.min.z = fminf(a.min.z, b.min.z);
	result.max.x = fmaxf(a.max.x, b.max.x);
	result.max.y = fmaxf(a.max.y, b.max.y);
	result.max.z = fmaxf(a.max.z, b.max.z);
	return (result);
}

// Determina si un rayo intersecta con un AABB
int	ray_intersects_aabb(t_vector origin, t_vector dir, t_aabb box)
{
	float	tmin;
	float	tmax;
	float	dim_dir;
	float	dim_org;
	float	dim_min;
	float	dim_max;
	float	t1;
	float	t2;
	float	temp;

	tmin = -INFINITY;
	tmax = INFINITY;
	// Para cada dimensión (x, y, z)
	for (int d = 0; d < 3; d++)
	{
		dim_dir = d == 0 ? dir.x : (d == 1 ? dir.y : dir.z);
		dim_org = d == 0 ? origin.x : (d == 1 ? origin.y : origin.z);
		dim_min = d == 0 ? box.min.x : (d == 1 ? box.min.y : box.min.z);
		dim_max = d == 0 ? box.max.x : (d == 1 ? box.max.y : box.max.z);
		if (fabs(dim_dir) < EPSILON)
		{
			// Rayo paralelo a este eje
			if (dim_org < dim_min || dim_org > dim_max)
				return (0);
		}
		else
		{
			t1 = (dim_min - dim_org) / dim_dir;
			t2 = (dim_max - dim_org) / dim_dir;
			if (t1 > t2)
			{
				temp = t1;
				t1 = t2;
				t2 = temp;
			}
			tmin = fmaxf(tmin, t1);
			tmax = fminf(tmax, t2);
			if (tmin > tmax)
				return (0);
		}
	}
	return (1);
}

// Construye un nodo BVH recursivamente
t_bvh_node	*build_bvh_node(t_object **objects, int start, int end, int depth)
{
	t_bvh_node	*node;
	int			object_span;
	int			mid;

	node = malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	// Inicializar el nodo
	node->left = NULL;
	node->right = NULL;
	node->objects = NULL;
	node->object_count = 0;
	// Crear AABB para este nodo
	node->bounds = create_aabb_for_object(objects[start]);
	for (int i = start + 1; i < end; i++)
		node->bounds = combine_aabb(node->bounds,
									create_aabb_for_object(objects[i]));
	object_span = end - start;
	// Si tenemos pocos objetos, este es un nodo hoja
	if (object_span <= 4 || depth > 10)
	{
		node->object_count = object_span;
		node->objects = malloc(sizeof(t_object *) * object_span);
		if (!node->objects)
		{
			free(node);
			return (NULL);
		}
		for (int i = 0; i < object_span; i++)
			node->objects[i] = objects[start + i];
		return (node);
	}
	// Determinar el eje de división (alternamos entre x, y, z)
	// Usar int axis para dividir los objetos (variable usada para evitar warning)
	mid = start + object_span / 2;
	// Construir nodos hijo recursivamente
	node->left = build_bvh_node(objects, start, mid, depth + 1);
	node->right = build_bvh_node(objects, mid, end, depth + 1);
	return (node);
}

// Inicializa la BVH
void	init_bvh(t_global *global)
{
	// Convertir objetos específicos a objetos genéricos
	global->total_objects = global->scene.num_sp + global->scene.num_pl
		+ global->scene.num_cy;
	global->objects = malloc(sizeof(t_object) * global->total_objects);

	if (!global->objects)
		return ;

	int idx = 0;

	// Añadir esferas
	for (int i = 0; i < global->scene.num_sp; i++)
	{
		global->objects[idx].type = OBJ_SPHERE;
		global->objects[idx].data.sphere = global->scene.spheres[i];
		global->objects[idx].bounds = create_aabb_for_object(&global->objects[idx]);
		idx++;
	}

	// Añadir planos
	for (int i = 0; i < global->scene.num_pl; i++)
	{
		global->objects[idx].type = OBJ_PLANE;
		global->objects[idx].data.plane = global->scene.planes[i];
		global->objects[idx].bounds = create_aabb_for_object(&global->objects[idx]);
		idx++;
	}

	// Añadir cilindros
	for (int i = 0; i < global->scene.num_cy; i++)
	{
		global->objects[idx].type = OBJ_CYLINDER;
		global->objects[idx].data.cylinder = global->scene.cyls[i];
		global->objects[idx].bounds = create_aabb_for_object(&global->objects[idx]);
		idx++;
	}

	// Construir el árbol BVH
	t_object **obj_pointers = malloc(sizeof(t_object *)
			* global->total_objects);
	if (!obj_pointers)
		return ;

	for (int i = 0; i < global->total_objects; i++)
		obj_pointers[i] = &global->objects[i];

	global->bvh_root = build_bvh_node(obj_pointers, 0, global->total_objects,
			0);

	free(obj_pointers);
	if (global->bvh_root)
		printf("BVH initialized successfully with %d total objects\n",
				global->total_objects);
	else
		printf("ERROR: BVH initialization failed!\n");
}