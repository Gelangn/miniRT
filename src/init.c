#include "minirt.h"

int	init(t_global *global)
{
	global->img.img = NULL;
	global->img.addr = NULL;
	global->scene = (t_scene){0};
	printf("Initializing MLX\n");
	global->vars.mlx_conn = mlx_init();
	if (!global->vars.mlx_conn)
		finish(global, ERR_MLX);
	printf("Initialized MLX OK\n");
	return (SUCCESS);
}

// Versión optimizada - eliminando el parámetro redundante
int	init_scene(t_global *global)
{
	t_scene	*scene;

	scene = &global->scene;
	scene->file_path = NULL;
	scene->fd = 0;
	scene->lines = NULL;
	scene->scale = 0;
	scene->ambient = (t_ambient){0};
	scene->cam = (t_camera){0};
	scene->light = (t_light){0};
	scene->ambient.init = 0;
	scene->cam.init = 0;
	scene->light.init = 0;
	scene->spheres = malloc(sizeof(t_plane) * MAX_SPHERES);
	if (!scene->spheres)
		finish(global, ERR_MEM);
	scene->num_sp = 0;
	scene->planes = malloc(sizeof(t_plane) * MAX_PLANES);
	if (!scene->planes)
		finish(global, ERR_MEM);
	scene->num_pl = 0;
	scene->cylinders = malloc(sizeof(t_plane) * MAX_CYLINDERS);
	if (!scene->cylinders)
		finish(global, ERR_MEM);
	scene->num_cy = 0;
	return (SUCCESS);
}

t_intersec	init_intersec(void)
{
	t_intersec	intersec;

	intersec.dist = INFINITY;
	intersec.point = (t_vector){0, 0, 0};
	intersec.obj_index = -1;
	intersec.obj_type = -1;
	return (intersec);
}

// Versión simplificada - usando current_cyl_vars en global
void	init_lateral_intersec_vars(t_global *global, int cy_id)
{
	t_cylinder	*cylinder;

	cylinder = &global->scene.cylinders[cy_id];
	// Normalizar eje del cilindro
	global->current_cyl_vars.axis = normalize(cylinder->orientation);
	// Calcular vector entre origen del rayo y base del cilindro
	global->current_cyl_vars.oc = subtract(global->current_ray_origin,
			cylinder->base);
	// Calcular proyecciones
	global->current_cyl_vars.dir_dot_axis = dot(global->current_ray_dir,
			global->current_cyl_vars.axis);
	global->current_cyl_vars.oc_dot_axis = dot(global->current_cyl_vars.oc,
			global->current_cyl_vars.axis);
	// Calcular componentes perpendiculares
	global->current_cyl_vars.dir_perp = subtract(global->current_ray_dir,
			multiply(global->current_cyl_vars.axis,
				global->current_cyl_vars.dir_dot_axis));
	global->current_cyl_vars.oc_perp = subtract(global->current_cyl_vars.oc,
			multiply(global->current_cyl_vars.axis,
				global->current_cyl_vars.oc_dot_axis));
}
