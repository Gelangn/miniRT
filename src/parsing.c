// incluir cabecera 42

#include "../inc/minirt.h"

// Función para dividir una cadena en tokens y convertir a float
float parse_float(char **str)
{
	char *token = strtok(*str, " ,");
	*str = NULL;
	return strtof(token, NULL);
}
// Función para dividir una cadena en tokens y convertir a int
int parse_int(char **str)
{
	char *token = ft_split(*str, ",");
	*str = NULL;
	return strtol(token, NULL, 10);
}

// Function to parse ambient light
void parse_ambient(t_scene *scene, char *line)
{
	char *str = line + 2;
	// cheeck if parse_float is between 0,0 and 1,0 *pending*
	scene->ambient.intensity = parse_float(&str);
	scene->ambient.color.r = parse_int(&str);
	scene->ambient.color.g = parse_int(&str);
	scene->ambient.color.b = parse_int(&str);
}

// Function to parse camera
void parse_camera(t_scene *scene, char *line)
{
	char *str = line + 2;
	scene->camera.position.x = parse_float(&str);
	scene->camera.position.y = parse_float(&str);
	scene->camera.position.z = parse_float(&str);
	scene->camera.orientation.x = parse_float(&str);
	scene->camera.orientation.y = parse_float(&str);
	scene->camera.orientation.z = parse_float(&str);
	scene->camera.fov = parse_int(&str);
}

// Function to parse light
void parse_light(t_scene *scene, char *line)
{
	char *str = line + 2;
	scene->light.position.x = parse_float(&str);
	scene->light.position.y = parse_float(&str);
	scene->light.position.z = parse_float(&str);
	scene->light.intensity = parse_float(&str);
}

// Function to parse sphere
void parse_sphere(t_scene *scene, char *line)
{
	t_sphere sphere;
	char *str = line + 3;
	sphere.center.x = parse_float(&str);
	sphere.center.y = parse_float(&str);
	sphere.center.z = parse_float(&str);
	sphere.radius = parse_float(&str);
	sphere.color.r = parse_int(&str);
	sphere.color.g = parse_int(&str);
	sphere.color.b = parse_int(&str);
	scene->spheres[scene->num_spheres++] = sphere;
}

// Function to parse plane
void parse_plane(t_scene *scene, char *line)
{
	t_plane plane;
	char *str = line + 3;
	plane.point.x = parse_float(&str);
	plane.point.y = parse_float(&str);
	plane.point.z = parse_float(&str);
	plane.normal.x = parse_float(&str);
	plane.normal.y = parse_float(&str);
	plane.normal.z = parse_float(&str);
	plane.color.r = parse_int(&str);
	plane.color.g = parse_int(&str);
	plane.color.b = parse_int(&str);
	scene->planes[scene->num_planes++] = plane;
}

// Function to parse cylinder
void parse_cylinder(t_scene *scene, char *line)
{
	t_cylinder cylinder;
	char *str = line + 3;
	cylinder.base.x = parse_float(&str);
	cylinder.base.y = parse_float(&str);
	cylinder.base.z = parse_float(&str);
	cylinder.orientation.x = parse_float(&str);
	cylinder.orientation.y = parse_float(&str);
	cylinder.orientation.z = parse_float(&str);
	cylinder.radius = parse_float(&str);
	cylinder.height = parse_float(&str);
	cylinder.color.r = parse_int(&str);
	cylinder.color.g = parse_int(&str);
	cylinder.color.b = parse_int(&str);
	scene->cylinders[scene->num_cylinders++] = cylinder;
}

// Function to read and parse the scene file
void read_scene(t_scene *scene, char *filename)
{
	int fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		if (scene->lines)
			free_scene(scene);
		finish(ERR_OPEN);
	}

	char *line_ptr;
	while ((line_ptr = get_next_line(fd)) != NULL)
	{
		if (line_ptr[0] == 'A')
			parse_ambient(scene, line_ptr);
		else if (line_ptr[0] == 'C')
			parse_camera(scene, line_ptr);
		else if (line_ptr[0] == 'L')
			parse_light(scene, line_ptr);
		else if (strncmp(line_ptr, "sp", 2) == 0)
			parse_sphere(scene, line_ptr);
		else if (strncmp(line_ptr, "pl", 2) == 0)
			parse_plane(scene, line_ptr);
		else if (strncmp(line_ptr, "cy", 2) == 0)
			parse_cylinder(scene, line_ptr);
		free(line_ptr);
	}
	close(fd);
}
