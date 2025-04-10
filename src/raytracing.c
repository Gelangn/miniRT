// incluir cabecera 42

#include "../inc/minirt.h"

float	cal_discriminant(t_vector oc, t_vector ray_dir, float radius)
{
	float	a;
	float	b;
	float	c;
	float	discriminant;

	a = dot(ray_dir, ray_dir);
	b = 2.0 * dot(oc, ray_dir);
	c = dot(oc, oc) - radius * radius;
	discriminant = b * b - 4 * a * c;
	return (discriminant);
}

t_intersec	col_sp(t_sphere *sphere, t_vector ray_origin, t_vector ray_dir)
{
	t_intersec	intersec;
	float		t1;
	float		t2;
	float		discriminant;
	t_vector	oc;

	intersec.dist = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	oc = subtract(ray_origin, sphere->center);
	discriminant = cal_discriminant(oc, ray_dir, sphere->radius);
	if (discriminant < 0)
		return (intersec);
	t1 = (-dot(ray_dir, oc) - sqrt(discriminant)) / dot(ray_dir, ray_dir);
	t2 = (-dot(ray_dir, oc) + sqrt(discriminant)) / dot(ray_dir, ray_dir);
	if (t1 > 0 && t2 > 0)
		intersec.dist = fmin(t1, t2);
	else if (t1 > 0)
		intersec.dist = t1;
	else if (t2 > 0)
		intersec.dist = t2;
	else
		return (intersec);
	intersec.point = add(ray_origin, multiply(ray_dir, intersec.dist));
	return (intersec);
}

t_intersec	col_pl(t_plane *plane, t_vector ray_origin, t_vector ray_dir)
{
	t_intersec	intersec;
	float		denom;
	float		t;
	t_vector	p0l0;

	intersec.dist = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	intersec.obj_index = -1;
	intersec.obj_type = -1;
	// Producto escalar entre normal del plano y dirección del rayo
	denom = dot(plane->normal, ray_dir);
	// Si es casi cero, el rayo es paralelo al plano
	if (fabs(denom) < EPSILON)
		return (intersec);
	// Vector desde el origen del rayo hasta un punto en el plano
	p0l0 = subtract(plane->point, ray_origin);
	// Calcula la distancia t a lo largo del rayo
	t = dot(p0l0, plane->normal) / denom;
	// Si t es negativo, la intersección está detrás del rayo
	if (t < 0)
		return (intersec);
	intersec.dist = t;
	intersec.point = add(ray_origin, multiply(ray_dir, t));
	intersec.obj_type = 1; // Tipo plano
	return (intersec);
}

t_intersec	calculate_cap_intersection(t_cylinder *cylinder,
		t_vector ray_origin, t_vector ray_dir, int cap_sign)
{
	t_intersec	intersec;
	t_vector	cap_center;
	t_vector	axis;
	float		t;
	t_vector	hit_point;
	float		distance_from_center;
	float		denom;
	t_vector	normal;

	// Inicializar intersección
	intersec.dist = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	intersec.obj_index = -1;
	intersec.obj_type = -1;
	// Normalizar el eje del cilindro
	axis = normalize(cylinder->orientation);
	// Calcular posición de la tapa directamente desde la base
	if (cap_sign == 1)
		cap_center = add(cylinder->base, multiply(axis, cylinder->height));
	else
		cap_center = cylinder->base;
	// Calcular t (distancia) para la intersección rayo-plano
	// El vector normal del plano es el eje del cilindro (para la tapa superior)
	// o su inverso (para la tapa inferior)
	normal = cap_sign == 1 ? axis : multiply(axis, -1);
	denom = dot(normal, ray_dir);
	// Si el rayo es paralelo al plano o viene desde atrás, no hay intersección
	if (fabs(denom) < EPSILON || denom > 0)
		// denom > 0 significa que golpea desde atrás
		return (intersec);
	// Calcular t
	t = dot(subtract(cap_center, ray_origin), normal) / denom;
	// Si t es negativo, la intersección está detrás del origen del rayo
	if (t < 0)
		return (intersec);
	// Calcular el punto de intersección
	hit_point = add(ray_origin, multiply(ray_dir, t));
	// Verificar si el punto está dentro del círculo de la tapa
	distance_from_center = magnitude(subtract(hit_point, cap_center));
	// Si la distancia es mayor que el radio, está fuera del círculo
	if (distance_from_center > cylinder->radius)
		return (intersec);
	// La intersección es válida
	intersec.dist = t;
	intersec.point = hit_point;
	intersec.obj_type = 2; // Tipo cilindro
	return (intersec);
}

t_intersec	calculate_lateral_intersection(t_cylinder *cylinder,
		t_vector ray_origin, t_vector ray_dir)
{
	t_intersec	intersec;
	t_vector	axis;
	t_vector	oc;
	t_vector	hit_point;
	float		hit_height;
	float		dir_dot_axis;
	float		oc_dot_axis;
	t_vector	dir_perp;
	t_vector	oc_perp;
	float		temp;
	float		t;
	t_vector	hit_to_axis;
	t_vector	center_at_height;
	t_vector	normal;

	float a, b, c, discriminant;
	float t1, t2;
	// Inicializar intersección
	intersec.dist = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	intersec.obj_index = -1;
	intersec.obj_type = -1;
	// Normalizar el eje del cilindro
	axis = normalize(cylinder->orientation);
	// Vector desde el origen del rayo hasta la base del cilindro
	oc = subtract(ray_origin, cylinder->base);
	// Proyección de la dirección del rayo sobre el eje del cilindro
	dir_dot_axis = dot(ray_dir, axis);
	// Proyección del vector oc sobre el eje del cilindro
	oc_dot_axis = dot(oc, axis);
	// Vector perpendicular a la dirección del rayo con respecto al eje
	dir_perp = subtract(ray_dir, multiply(axis, dir_dot_axis));
	// Vector perpendicular a oc con respecto al eje
	oc_perp = subtract(oc, multiply(axis, oc_dot_axis));
	// Coeficientes de la ecuación cuadrática
	a = dot(dir_perp, dir_perp);
	b = 2 * dot(dir_perp, oc_perp);
	c = dot(oc_perp, oc_perp) - (cylinder->radius * cylinder->radius);
	// Si a es casi cero, el rayo es paralelo a la superficie del cilindro
	if (fabs(a) < EPSILON)
		return (intersec);
	// Calcular el discriminante
	discriminant = b * b - 4 * a * c;
	// Si el discriminante es negativo, no hay intersección
	if (discriminant < 0)
		return (intersec);
	// Calcular los puntos de intersección
	t1 = (-b - sqrt(discriminant)) / (2 * a);
	t2 = (-b + sqrt(discriminant)) / (2 * a);
	// Ordenar t1 y t2 para que t1 <= t2
	if (t1 > t2)
	{
		temp = t1;
		t1 = t2;
		t2 = temp;
	}
	// Comprobar las intersecciones dentro de la altura del cilindro
	t = t1;
	for (int i = 0; i < 2; i++)
	{
		if (t < 0) // Pasar a la siguiente intersección si está detrás del rayo
		{
			t = t2;
			continue ;
		}
		// Calcular el punto de intersección
		hit_point = add(ray_origin, multiply(ray_dir, t));
		// Calcular la altura del punto de intersección relativa a la base del cilindro
		hit_height = dot(subtract(hit_point, cylinder->base), axis);
		// Verificar si la intersección está dentro de la altura del cilindro
		if (hit_height >= -EPSILON && hit_height <= cylinder->height + EPSILON)
		{
			// Calcular la normal en el punto de intersección (apuntando hacia afuera)
			hit_to_axis = multiply(axis, hit_height);
			center_at_height = add(cylinder->base, hit_to_axis);
			normal = normalize(subtract(hit_point, center_at_height));
			// Verificar si la normal apunta en dirección contraria al rayo (golpe frontal)
			if (dot(normal, ray_dir) < 0)
			{
				intersec.dist = t;
				intersec.point = hit_point;
				intersec.obj_type = 2; // Tipo cilindro
				return (intersec);
			}
		}
		t = t2; // Probar con la segunda intersección
	}
	return (intersec);
}

t_intersec	col_cy(t_cylinder *cylinder, t_vector ray_origin, t_vector ray_dir)
{
	t_intersec	intersec;
	t_intersec	lateral_intersec;
	t_intersec	top_cap_intersec;
	t_intersec	bottom_cap_intersec;

	// Inicializar intersección con valores por defecto
	intersec.dist = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	intersec.obj_index = -1;
	intersec.obj_type = -1;
	// 1. Calcular intersección con la superficie lateral (código existente)
	lateral_intersec = calculate_lateral_intersection(cylinder, ray_origin,
			ray_dir);
	// 2. Calcular intersección con la tapa superior
	top_cap_intersec = calculate_cap_intersection(cylinder, ray_origin, ray_dir,
			1);
	// 3. Calcular intersección con la tapa inferior
	bottom_cap_intersec = calculate_cap_intersection(cylinder, ray_origin,
			ray_dir, -1);
	// 4. Determinar cuál es la intersección más cercana
	if (lateral_intersec.dist < intersec.dist)
		intersec = lateral_intersec;
	if (top_cap_intersec.dist < intersec.dist)
		intersec = top_cap_intersec;
	if (bottom_cap_intersec.dist < intersec.dist)
		intersec = bottom_cap_intersec;
	return (intersec);
}

t_vector	get_ray_direction(t_camera cam, int pixel_x, int pixel_y)
{
	float		u;
	float		v;
	float		scrn_w;
	float		scrn_h;
	float		aspect_ratio;
	t_vector	forward;
	t_vector	right;
	t_vector	up;
	t_vector	ray_dir;

	aspect_ratio = (float)WIN_W / (float)WIN_H;
	scrn_w = 2.0 * DSCR * tan((cam.fov * PI / 180.0) / 2.0);
	scrn_h = scrn_w / aspect_ratio;
	forward = normalize(cam.orientation);
	right = normalize(cross((t_vector){0, 1, 0}, forward));
	up = cross(forward, right);
	u = (2 * ((pixel_x + 0.5) / WIN_W) - 1) * scrn_w / 2;
	v = (1 - 2 * ((pixel_y + 0.5) / WIN_H)) * scrn_h / 2;
	ray_dir = normalize(add(add(multiply(right, u), multiply(up, v)), forward));
	return (ray_dir);
}

t_intersec	find_closest_intersec(t_global *global, t_vector ray_origin,
		t_vector ray_dir)
{
	t_sphere	*sphere;
	t_plane		*plane;
	t_cylinder	*cylinder;
	t_intersec	temp_intersec;
	t_intersec	closest_intersec;
	int			i;

	closest_intersec.dist = INFINITY;
	closest_intersec.point = (t_vector){0, 0, 0};
	closest_intersec.obj_index = -1;
	closest_intersec.obj_type = -1;
	// Verificar intersecciones con esferas
	sphere = global->scene.spheres;
	i = -1;
	while (++i < global->scene.num_sp)
	{
		temp_intersec = col_sp(&sphere[i], ray_origin, ray_dir);
		if (temp_intersec.dist < closest_intersec.dist)
		{
			closest_intersec = temp_intersec;
			closest_intersec.obj_index = i;
			closest_intersec.obj_type = 0; // Tipo esfera
		}
	}
	// Verificar intersecciones con planos
	plane = global->scene.planes;
	i = -1;
	while (++i < global->scene.num_pl)
	{
		temp_intersec = col_pl(&plane[i], ray_origin, ray_dir);
		if (temp_intersec.dist < closest_intersec.dist)
		{
			closest_intersec = temp_intersec;
			closest_intersec.obj_index = i;
			closest_intersec.obj_type = 1; // Tipo plano
		}
	}
	// Verificar intersecciones con cilindros
	cylinder = global->scene.cylinders;
	i = -1;
	while (++i < global->scene.num_cy)
	{
		temp_intersec = col_cy(&cylinder[i], ray_origin, ray_dir);
		if (temp_intersec.dist < closest_intersec.dist)
		{
			closest_intersec = temp_intersec;
			closest_intersec.obj_index = i;
			closest_intersec.obj_type = 2; // Tipo cilindro
		}
	}
	return (closest_intersec);
}

t_intersec	calculate_pixel(t_global *global, int pixel_x, int pixel_y)
{
	t_vector	ray_dir;
	t_camera	camera;
	t_intersec	intersec;

	camera = global->scene.cam;
	ray_dir = get_ray_direction(camera, pixel_x, pixel_y);
	intersec = find_closest_intersec(global, camera.pos, ray_dir);
	return (intersec);
}

void	render_pixel(t_global *global, t_intersec intersec, t_img *img, int x,
		int y)
{
	int			color;
	t_vector	normal;
	float		dot_normal_axis;

	if (intersec.obj_type == 0 && intersec.obj_index >= 0) // Esfera
		color = rgb_to_int(global->scene.spheres[intersec.obj_index].color);
	else if (intersec.obj_type == 1 && intersec.obj_index >= 0) // Plano
		color = rgb_to_int(global->scene.planes[intersec.obj_index].color);
	else if (intersec.obj_type == 2 && intersec.obj_index >= 0) // Cilindro
	{
		// Determinar si es tapa o superficie lateral comparando normales
		normal = get_surface_normal(global, intersec);
		dot_normal_axis = fabs(dot(normal,
					global->scene.cylinders[intersec.obj_index].orientation));
		if (dot_normal_axis > 0.99) // Es una tapa
			color = RED;            // Rojo para tapas
		else
			color = CYAN; // Azul para superficie lateral
	}
	else
		color = DARK_GREY; // Color gris oscuro para fondo
	pixel_put(img, x, y, color);
}

void	render(t_global *global)
{
	int			x;
	int			y;
	int			i;
	t_intersec	*intersections;

	// Asignar memoria para almacenar todas las intersecciones
	intersections = malloc(WIN_W * WIN_H * sizeof(t_intersec));
	if (!intersections)
		return ;
	// Primer paso: calcular todas las intersecciones
	i = 0;
	x = -1;
	while (++x < WIN_W)
	{
		y = -1;
		while (++y < WIN_H)
		{
			intersections[i++] = calculate_pixel(global, x, y);
		}
	}
	printf("Total calculate rays: %d\n", i);
	// Segundo paso: pintar los píxeles según las intersecciones calculadas
	i = 0;
	x = -1;
	while (++x < WIN_W)
	{
		y = -1;
		while (++y < WIN_H)
		{
			render_pixel(global, intersections[i++], &global->img, x, y);
		}
	}
	free(intersections);
}

// Añade esta función

t_vector	get_surface_normal(t_global *global, t_intersec intersec)
{
	t_vector	normal;
	t_sphere	sphere;
	t_plane		plane;
	t_cylinder	cylinder;
	t_vector	axis;
	float		hit_height;
	t_vector	center_at_height;

	normal = (t_vector){0, 0, 0};
	if (intersec.obj_type == 0) // Esfera
	{
		// La normal en cualquier punto de una esfera apunta desde el centro hacia el punto
		sphere = global->scene.spheres[intersec.obj_index];
		normal = normalize(subtract(intersec.point, sphere.center));
	}
	else if (intersec.obj_type == 1) // Plano
	{
		// La normal de un plano es constante en todos sus puntos
		plane = global->scene.planes[intersec.obj_index];
		normal = normalize(plane.normal);
	}
	else if (intersec.obj_type == 2) // Cilindro
	{
		cylinder = global->scene.cylinders[intersec.obj_index];
		axis = normalize(cylinder.orientation);
		// Calcular altura del punto relativa a la base
		hit_height = dot(subtract(intersec.point, cylinder.base), axis);
		// Si estamos muy cerca de la base o la tapa, es un punto en la tapa
		if (hit_height < EPSILON)
		{
			// Tapa inferior - normal hacia abajo
			normal = multiply(axis, -1);
		}
		else if (fabs(hit_height - cylinder.height) < EPSILON)
		{
			// Tapa superior - normal hacia arriba
			normal = axis;
		}
		else
		{
			// Punto en la superficie lateral
			// Proyectar el punto en el eje y calcular la normal
			center_at_height = add(cylinder.base, multiply(axis, hit_height));
			normal = normalize(subtract(intersec.point, center_at_height));
		}
	}
	return (normal);
}
