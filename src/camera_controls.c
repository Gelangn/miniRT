#include "../inc/minirt.h"

// Mover la cámara en una dirección específica
void	move_camera(t_global *global, t_vector direction, float distance)
{
	t_vector	movement;

	movement = multiply(direction, distance);
	global->scene.cam.pos = add(global->scene.cam.pos, movement);
}

// Rotar la cámara alrededor de un eje
void	rotate_camera(t_global *global, t_vector axis, float angle)
{
	t_vector	z;
	t_vector	x;
	t_vector	y;
	float		c;
	float		s;

	z = normalize(global->scene.cam.orientation);
	x = normalize(cross(z, (t_vector){0, -1, 0}));
	y = normalize(cross(x, z));
	// Crear matriz de rotación (simplificada)
	if (comp_floats(magnitude(axis), 0))
		return ;
	axis = normalize(axis);
	c = cosf(angle);
	s = sinf(angle);
	// Aplicar rotación
	if (comp_floats(axis.x, 1))
		global->scene.cam.orientation = add(multiply(z, c), multiply(y, s));
	else if (comp_floats(axis.y, 1))
		global->scene.cam.orientation = add(multiply(z, c), multiply(x, s));
	global->scene.cam.orientation = normalize(global->scene.cam.orientation);
}
