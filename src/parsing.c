// incluir cabecera 42

#include "../inc/minirt.h"

// Function to parse ambient light
void parse_ambient(t_scene *scene, char *line)
{
	sscanf(line, "A %f %d,%d,%d", &scene->ambient.intensity, &scene->ambient.color.r, &scene->ambient.color.g, &scene->ambient.color.b);
}

// Function to parse camera
void parse_camera(t_scene *scene, char *line)
{
	sscanf(line, "C %f,%f,%f %f,%f,%f %d", &scene->camera.position.x, &scene->camera.position.y, &scene->camera.position.z, &scene->camera.orientation.x, &scene->camera.orientation.y, &scene->camera.orientation.z, &scene->camera.fov);
}

// Function to parse light
void parse_light(t_scene *scene, char *line)
{
	sscanf(line, "L %f,%f,%f %f", &scene->light.position.x, &scene->light.position.y, &scene->light.position.z, &scene->light.intensity);
}

// Function to parse sphere
void parse_sphere(t_scene *scene, char *line)
{
	t_sphere sphere;
	sscanf(line, "sp %f,%f,%f %f %d,%d,%d", &sphere.center.x, &sphere.center.y, &sphere.center.z, &sphere.radius, &sphere.color.r, &sphere.color.g, &sphere.color.b);
	scene->spheres[scene->num_spheres++] = sphere;
}

// Function to parse plane
void parse_plane(t_scene *scene, char *line)
{
	t_plane plane;
	sscanf(line, "pl %f,%f,%f %f,%f,%f %d,%d,%d", &plane.point.x, &plane.point.y, &plane.point.z, &plane.normal.x, &plane.normal.y, &plane.normal.z, &plane.color.r, &plane.color.g, &plane.color.b);
	scene->planes[scene->num_planes++] = plane;
}

// Function to parse cylinder
void parse_cylinder(t_scene *scene, char *line)
{
	t_cylinder cylinder;
	sscanf(line, "cy %f,%f,%f %f,%f,%f %f %f %d,%d,%d", &cylinder.base.x, &cylinder.base.y, &cylinder.base.z, &cylinder.orientation.x, &cylinder.orientation.y, &cylinder.orientation.z, &cylinder.radius, &cylinder.height, &cylinder.color.r, &cylinder.color.g, &cylinder.color.b);
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

	FILE *file = fdopen(fd, "r");
	if (!file)
	{
		close(fd);
		if (scene->lines)
			free_scene(scene);
		finish(ERR_OPEN);
	}

	char *line_ptr;
	while ((line_ptr = get_next_line(fileno(file))) != NULL)
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
	}
	fclose(file);
}
