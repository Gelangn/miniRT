// incluir cabecera 42

#include "../inc/minirt.h"
#include "../inc/parsing.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

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
void read_scene(t_scene *scene, const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file)
	{
		perror("Error opening file");
		exit(EXIT_FAILURE);
	}

	char line[256];
	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == 'A')
			parse_ambient(scene, line);
		else if (line[0] == 'C')
			parse_camera(scene, line);
		else if (line[0] == 'L')
			parse_light(scene, line);
		else if (strncmp(line, "sp", 2) == 0)
			parse_sphere(scene, line);
		else if (strncmp(line, "pl", 2) == 0)
			parse_plane(scene, line);
		else if (strncmp(line, "cy", 2) == 0)
			parse_cylinder(scene, line);
	}

	fclose(file);
}
/* t_scene	read_scene(t_scene *scene, char *file_path)
{
	scene->file_path = file_path;
	get_scene_size(scene);
	get_scene(scene);
	//scene->nr_elems = scene->width * scene->height;
	fill_points(scene);
	return (*scene);
} */