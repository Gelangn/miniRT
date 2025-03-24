// incluir cabecera 42

#include "../inc/minirt.h"


void	check_scene(t_global *global, t_scene *scene)
{
	if (scene->ambient.initialized == 0 || scene->camera.initialized == 0
		|| scene->light.initialized == 0)
		finish(global, ERR_SCENE);
	if (scene->num_spheres < 0 || scene->num_spheres > MAX_SPHERES)
		finish(global, ERR_SPHERE);
	if (scene->num_planes < 0 || scene->num_planes > MAX_PLANES)
		finish(global, ERR_PLANE);
	if (scene->num_cylinders < 0 || scene->num_cylinders > MAX_CYLINDERS)
		finish(global, ERR_CYLINDER);
}
// Function to read and parse the scene file
void	read_scene(t_global *global)
{
	char	*line_ptr;
	int		fd;
	t_scene	*scene;
	char	*filename;

	filename = global->scene.file_path;
	scene = &global->scene;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		finish(global, ERR_OPEN);
	if ((line_ptr = get_next_line(fd)) == NULL)
		finish(global, ERR_READ);
	else
	{
		while (line_ptr)
		{
			if (line_ptr[0] == 'A')
				parse_ambient(global, scene, line_ptr);
			else if (line_ptr[0] == 'C')
				parse_camera(global, scene, line_ptr);
			else if (line_ptr[0] == 'L')
				parse_light(global, scene, line_ptr);
			else if (strncmp(line_ptr, "sp", 2) == 0)
				parse_sphere(global, scene, line_ptr);
			else if (strncmp(line_ptr, "pl", 2) == 0)
				parse_plane(global, scene, line_ptr);
			else if (strncmp(line_ptr, "cy", 2) == 0)
				parse_cylinder(global, scene, line_ptr);
			free(line_ptr);
			line_ptr = get_next_line(fd);
		}
	}
	check_scene(global, scene);
	close(fd);
}
