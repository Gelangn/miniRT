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

	intersec.distance = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	oc = subtract(ray_origin, sphere->center);
	discriminant = cal_discriminant(oc, ray_dir, sphere->radius);
	if (discriminant < 0)
		return (intersec);
	t1 = (-dot(ray_dir, oc) - sqrt(discriminant)) / dot(ray_dir, ray_dir);
	t2 = (-dot(ray_dir, oc) + sqrt(discriminant)) / dot(ray_dir, ray_dir);
	if (t1 > 0 && t2 > 0)
		intersec.distance = fmin(t1, t2);
	else if (t1 > 0)
		intersec.distance = t1;
	else if (t2 > 0)
		intersec.distance = t2;
	else
		return (intersec);
	intersec.point = add(ray_origin, multiply(ray_dir, intersec.distance));
	return (intersec);
}

t_intersec col_pl(t_plane *plane, t_vector ray_origin, t_vector ray_dir)
{
    t_intersec intersec;
    float denom;
    float t;
    t_vector p0l0;

    intersec.distance = INFINITY;
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
    
    intersec.distance = t;
    intersec.point = add(ray_origin, multiply(ray_dir, t));
    intersec.obj_type = 1; // Tipo plano
    
    return (intersec);
}

t_intersec col_cy(t_cylinder *cylinder, t_vector ray_origin, t_vector ray_dir)
{
    t_intersec intersec;
    t_vector oc, axis_proj_oc, d_proj_axis;
    float a, b, c, discriminant, t1, t2;
    
    intersec.distance = INFINITY;
    intersec.point = (t_vector){0, 0, 0};
    intersec.obj_index = -1;
    intersec.obj_type = -1;
    
    // Normalizar el eje del cilindro
    t_vector axis = normalize(cylinder->orientation);
    
    // Vector desde el origen del cilindro al origen del rayo
    oc = subtract(ray_origin, cylinder->base);
    
    // Proyectar oc en el eje del cilindro
    float oc_dot_axis = dot(oc, axis);
    axis_proj_oc = multiply(axis, oc_dot_axis);
    
    // Proyectar ray_dir en el eje del cilindro
    float d_dot_axis = dot(ray_dir, axis);
    d_proj_axis = multiply(axis, d_dot_axis);
    
    // Componente de oc perpendicular al eje
    t_vector perp_oc = subtract(oc, axis_proj_oc);
    // Componente de ray_dir perpendicular al eje
    t_vector perp_dir = subtract(ray_dir, d_proj_axis);
    
    // Calcular coeficientes de la ecuación cuadrática
    a = dot(perp_dir, perp_dir);
    b = 2.0 * dot(perp_dir, perp_oc);
    c = dot(perp_oc, perp_oc) - cylinder->radius * cylinder->radius;
    
    discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
        return (intersec);
        
    t1 = (-b - sqrt(discriminant)) / (2 * a);
    t2 = (-b + sqrt(discriminant)) / (2 * a);
    
    // Encontrar la intersección válida más cercana
    float t = INFINITY;
    if (t1 > 0 && t2 > 0)
        t = fmin(t1, t2);
    else if (t1 > 0)
        t = t1;
    else if (t2 > 0)
        t = t2;
    else
        return (intersec);
        
    // Calcular punto de intersección
    t_vector hit_point = add(ray_origin, multiply(ray_dir, t));
    
    // Verificar si el punto está dentro de la altura del cilindro
    t_vector hit_to_center = subtract(hit_point, cylinder->base);
    float height_proj = dot(hit_to_center, axis);
    
    if (fabs(height_proj) > cylinder->height / 2)
        return (intersec);
        
    intersec.distance = t;
    intersec.point = hit_point;
    intersec.obj_type = 2; // Tipo cilindro
    
    return (intersec);
}

t_vector	get_ray_direction(t_camera camera, int pixel_x, int pixel_y)
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
	scrn_w = 2.0 * tan((camera.fov * PI / 180.0) / 2.0);
	scrn_h = scrn_w / aspect_ratio;
	forward = normalize(camera.orientation);
	right = normalize(cross((t_vector){0, 1, 0}, forward));
	up = cross(forward, right);
	u = (2 * ((pixel_x + 0.5) / WIN_W) - 1) * scrn_w / 2;
	v = (1 - 2 * ((pixel_y + 0.5) / WIN_H)) * scrn_h / 2;
	ray_dir = normalize(add(add(multiply(right, u), multiply(up, v)), forward));
	return (ray_dir);
}

t_intersec find_closest_intersec(t_global *global, t_vector ray_origin, t_vector ray_dir)
{
    t_sphere *sphere;
    t_plane *plane;
    t_cylinder *cylinder;
    t_intersec temp_intersec;
    t_intersec closest_intersec;
    int i;

    closest_intersec.distance = INFINITY;
    closest_intersec.point = (t_vector){0, 0, 0};
    closest_intersec.obj_index = -1;
    closest_intersec.obj_type = -1;
    
    // Verificar intersecciones con esferas
    sphere = global->scene.spheres;
    i = 0;
    while (i < global->scene.num_sp)
    {
        temp_intersec = col_sp(&sphere[i], ray_origin, ray_dir);
        if (temp_intersec.distance < closest_intersec.distance)
        {
            closest_intersec = temp_intersec;
            closest_intersec.obj_index = i;
            closest_intersec.obj_type = 0; // Tipo esfera
        }
        i++;
    }
    
    // Verificar intersecciones con planos
    plane = global->scene.planes;
    i = 0;
    while (i < global->scene.num_pl)
    {
        temp_intersec = col_pl(&plane[i], ray_origin, ray_dir);
        if (temp_intersec.distance < closest_intersec.distance)
        {
            closest_intersec = temp_intersec;
            closest_intersec.obj_index = i;
            closest_intersec.obj_type = 1; // Tipo plano
        }
        i++;
    }
    
    // Verificar intersecciones con cilindros
    cylinder = global->scene.cylinders;
    i = 0;
    while (i < global->scene.num_cy)
    {
        temp_intersec = col_cy(&cylinder[i], ray_origin, ray_dir);
        if (temp_intersec.distance < closest_intersec.distance)
        {
            closest_intersec = temp_intersec;
            closest_intersec.obj_index = i;
            closest_intersec.obj_type = 2; // Tipo cilindro
        }
        i++;
    }
    
    return (closest_intersec);
}

t_vector render_pixel(t_global *global, int pixel_x, int pixel_y)
{
    t_vector ray_dir;
    t_vector hit_point;
    t_camera camera;
    t_intersec intersec;
    int color;

    camera = global->scene.camera;
    ray_dir = get_ray_direction(camera, pixel_x, pixel_y);
    intersec = find_closest_intersec(global, camera.position, ray_dir);
    hit_point = intersec.point;
    
    // Asignar color según el tipo de objeto intersectado
    if (intersec.obj_type == 0 && intersec.obj_index >= 0) // Esfera
    {
        color = rgb_to_int(global->scene.spheres[intersec.obj_index].color);
        pixel_put(&global->img, pixel_x, pixel_y, color);
    }
    else if (intersec.obj_type == 1 && intersec.obj_index >= 0) // Plano
    {
        color = rgb_to_int(global->scene.planes[intersec.obj_index].color);
        pixel_put(&global->img, pixel_x, pixel_y, color);
    }
    else if (intersec.obj_type == 2 && intersec.obj_index >= 0) // Cilindro
    {
        color = rgb_to_int(global->scene.cylinders[intersec.obj_index].color);
        pixel_put(&global->img, pixel_x, pixel_y, color);
    }
    
    return (hit_point);
}

void	render(t_global *global)
{
	int	x;
	int	y;
	int	i;

	x = -1;
	y = -1;
	i = 0;
	while (++x < WIN_W)
	{
		y = -1;
		while (++y < WIN_H)
		{
			i++;
			render_pixel(global, x, y);
		}
	}
	printf("Total rays: %d\n", i); // Verificar conteo final
}
