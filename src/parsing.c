// incluir cabecera 42

#include "../inc/minirt.h"

/* char *tokenize(char *str, char delim)
{
	char *token = ft_split(str, delim);
	while (token)
	{
		printf("%s\n", token);
		token = (NULL, delim);
	}
	return token;
}
 */
/* // Function to split a string into tokens and convert to float
float parse_float(char **str)
{
	char **token = ft_split(*str, ',');
	return roundingf(ft_atof(*token), 2);
}
// Function to split a string into tokens and convert to int
int parse_int(char **str)
{
	char **token = ft_split(*str, ',');
	return (ft_atoi(*token));
} */
// Function to replace tabs with spaces
static void replace_tabs_with_spaces(char *str)
{
	while (*str)
	{
		if (*str == '\t')
			*str = ' ';
		str++;
	}
}
// Function to round a float number to a given number of decimals
float roundingf(float value, int decimals)
{
	float factor = powf(10.0, decimals);
	return roundf(value * factor) / factor;
}
// Function to parse ambient light
void parse_ambient(t_global *global, t_scene *scene, char *line)
{
	char **tokens;
	char **color_tokens;

	replace_tabs_with_spaces(line); // Reemplazar tabuladores por espacios
	tokens = ft_split(line, ' ');
	if (!tokens || scene->ambient.initialized)
		finish(global, ERR_AMBIENT); // Comprobar si token es nulo
	tokens++;
	if (!*tokens)
		finish(global, ERR_AMBIENT); // Comprobar si token es nulo
	// Parse intensity
	scene->ambient.intensity = roundingf(ft_atof(*tokens), 2);
	if (scene->ambient.intensity < 0.0 || scene->ambient.intensity > 1.0)
		finish(global, ERR_AMBIENT);
	// Parse color
	tokens++;
	if (!*tokens)
		finish(global, ERR_AMBIENT); // Comprobar si tokens es nulo
	color_tokens = ft_split(*tokens, ',');
	if (!color_tokens)
		finish(global, ERR_AMBIENT); // Comprobar si color_tokens es nulo
	scene->ambient.color.r = ft_atoi(color_tokens[0]);
	scene->ambient.color.g = ft_atoi(color_tokens[1]);
	scene->ambient.color.b = ft_atoi(color_tokens[2]);
	free(color_tokens);
	scene->ambient.initialized = 1;
}
// Function to parse camera
void parse_camera(t_global *global, t_scene *scene, char *line)
{
	replace_tabs_with_spaces(line); // Reemplazar tabuladores por espacios
	char **tokens = ft_split(line, ' ');
	if (!tokens || scene->camera.initialized == 1)
		finish(global, ERR_CAMERA); // Comprobar si tokens es nulo o si ya se ha leído la cámara
	tokens++;
	if (!*tokens)
		finish(global, ERR_CAMERA); // Comprobar si tokens es nulo

	// Parsear posición
	char **position_tokens = ft_split(*tokens, ',');
	if (!position_tokens)
		finish(global, ERR_CAMERA); // Comprobar si position_tokens es nulo
	scene->camera.position.x = roundingf(ft_atof(position_tokens[0]), 2);
	scene->camera.position.y = roundingf(ft_atof(position_tokens[1]), 2);
	scene->camera.position.z = roundingf(ft_atof(position_tokens[2]), 2);
	free(position_tokens);

	// Parsear orientación
	tokens++;
	if (!*tokens)
		finish(global, ERR_CAMERA); // Comprobar si tokens es nulo
	char **orientation_tokens = ft_split(*tokens, ',');
	if (!orientation_tokens)
		finish(global, ERR_CAMERA); // Comprobar si orientation_tokens es nulo
	scene->camera.orientation.x = roundingf(ft_atof(orientation_tokens[0]), 2);
	scene->camera.orientation.y = roundingf(ft_atof(orientation_tokens[1]), 2);
	scene->camera.orientation.z = roundingf(ft_atof(orientation_tokens[2]), 2);
	free(orientation_tokens);

	// Parsear FOV
	tokens++;
	if (!*tokens)
		finish(global, ERR_CAMERA); // Comprobar si tokens es nulo
	scene->camera.fov = ft_atoi(*tokens);
	if (scene->camera.fov < 0 || scene->camera.fov > 180)
		finish(global, ERR_CAMERA);
	scene->camera.initialized = 1;
}
// Function to parse light
void parse_light(t_global *global, t_scene *scene, char *line)
{
	replace_tabs_with_spaces(line);
	char **tokens = ft_split(line, ' ');
	if (!tokens || scene->light.initialized == 1)
		finish(global, ERR_LIGHT); // Comprobar si tokens es nulo o si ya se ha leído la luz
	tokens++;
	if (!*tokens)
		finish(global, ERR_LIGHT); // Comprobar si tokens es nulo
	// Parse position
	char **position_tokens = ft_split(*tokens, ',');
	if (!position_tokens)
		finish(global, ERR_LIGHT); // Comprobar si position_tokens es nulo
	scene->light.position.x = roundingf(ft_atof(position_tokens[0]), 2);
	scene->light.position.y = roundingf(ft_atof(position_tokens[1]), 2);
	scene->light.position.z = roundingf(ft_atof(position_tokens[2]), 2);
	free(position_tokens);
	// Parse intensity
	tokens++;
	if (!*tokens)
		finish(global, ERR_LIGHT); // Comprobar si tokens es nulo
	scene->light.intensity = roundingf(ft_atof(*tokens), 2);
	if (scene->light.intensity < 0.0 || scene->light.intensity > 1.0)
		finish(global, ERR_LIGHT);
	scene->light.initialized = 1;
}
// Function to parse sphere
void parse_sphere(t_global *global, t_scene *scene, char *line)
{
	t_sphere sphere;
	replace_tabs_with_spaces(line);
	char **tokens = ft_split(line, ' ');
	if (!tokens)
		finish(global, ERR_SPHERE); // Comprobar si tokens es nulo
	tokens++;
	if (!*tokens)
		finish(global, ERR_SPHERE); // Comprobar si tokens es nulo

	// Parsear centro
	char **center_tokens = ft_split(*tokens, ',');
	if (!center_tokens)
		finish(global, ERR_SPHERE); // Comprobar si center_tokens es nulo
	sphere.center.x = roundingf(ft_atof(center_tokens[0]), 2);
	sphere.center.y = roundingf(ft_atof(center_tokens[1]), 2);
	sphere.center.z = roundingf(ft_atof(center_tokens[2]), 2);
	free(center_tokens);

	// Parsear radio
	tokens++;
	if (!*tokens)
		finish(global, ERR_SPHERE); // Comprobar si tokens es nulo
	sphere.radius = roundingf(ft_atof(*tokens), 2);

	// Parsear color
	tokens++;
	if (!*tokens)
		finish(global, ERR_SPHERE); // Comprobar si tokens es nulo
	char **color_tokens = ft_split(*tokens, ',');
	if (!color_tokens)
		finish(global, ERR_SPHERE); // Comprobar si color_tokens es nulo
	sphere.color.r = ft_atoi(color_tokens[0]);
	sphere.color.g = ft_atoi(color_tokens[1]);
	sphere.color.b = ft_atoi(color_tokens[2]);
	free(color_tokens);

	scene->spheres[scene->num_spheres++] = sphere;
}
// Function to parse plane
void parse_plane(t_global *global, t_scene *scene, char *line)
{
	t_plane plane;
	replace_tabs_with_spaces(line);
	char **tokens = ft_split(line, ' ');
	if (!tokens)
		finish(global, ERR_PLANE); // Comprobar si tokens es nulo
	tokens++;			   // saltar el primer token

	// Parsear punto
	char **point_tokens = ft_split(*tokens, ',');
	if (!point_tokens)
		finish(global, ERR_PLANE); // Comprobar si point_tokens es nulo
	plane.point.x = roundingf(ft_atof(point_tokens[0]), 2);
	plane.point.y = roundingf(ft_atof(point_tokens[1]), 2);
	plane.point.z = roundingf(ft_atof(point_tokens[2]), 2);
	free(point_tokens);

	// Parsear normal
	tokens++;
	if (!*tokens)
		finish(global, ERR_PLANE); // Comprobar si tokens es nulo
	char **normal_tokens = ft_split(*tokens, ',');
	if (!normal_tokens)
		finish(global, ERR_PLANE); // Comprobar si normal_tokens es nulo
	plane.normal.x = roundingf(ft_atof(normal_tokens[0]), 2);
	plane.normal.y = roundingf(ft_atof(normal_tokens[1]), 2);
	plane.normal.z = roundingf(ft_atof(normal_tokens[2]), 2);
	free(normal_tokens);

	// Parsear color
	tokens++;
	if (!*tokens)
		finish(global, ERR_PLANE); // Comprobar si tokens es nulo
	char **color_tokens = ft_split(*tokens, ',');
	if (!color_tokens)
		finish(global, ERR_PLANE); // Comprobar si color_tokens es nulo
	plane.color.r = ft_atoi(color_tokens[0]);
	plane.color.g = ft_atoi(color_tokens[1]);
	plane.color.b = ft_atoi(color_tokens[2]);
	free(color_tokens);

	if (!scene)
	{
		printf("Error: scene es nulo\n");
		finish(global, ERR_PLANE);
	}
	if (!scene->planes)
	{
		printf("Error: scene->planes es nulo\n");
		finish(global, ERR_PLANE);
	}
	if (scene->num_planes >= MAX_PLANES)
	{ // Suponiendo que hay un límite MAX_PLANES
		printf("Error: num_planes excede el límite\n");
		finish(global, ERR_PLANE);
	}

	scene->planes[scene->num_planes++] = plane;
}
// Function to parse cylinder
void parse_cylinder(t_global *global, t_scene *scene, char *line)
{
	t_cylinder cylinder;
	replace_tabs_with_spaces(line); // Reemplazar tabuladores por espacios
	char **tokens = ft_split(line, ' ');
	if (!tokens)
		finish(global, ERR_CYLINDER); // Comprobar si tokens es nulo
	tokens++;
	if (!*tokens)
		finish(global, ERR_CYLINDER); // Comprobar si tokens es nulo

	// Parsear base
	char **base_tokens = ft_split(*tokens, ',');
	if (!base_tokens)
		finish(global, ERR_CYLINDER); // Comprobar si base_tokens es nulo
	cylinder.base.x = roundingf(ft_atof(base_tokens[0]), 2);
	cylinder.base.y = roundingf(ft_atof(base_tokens[1]), 2);
	cylinder.base.z = roundingf(ft_atof(base_tokens[2]), 2);
	free(base_tokens);

	// Parsear orientación
	tokens++;
	if (!*tokens)
		finish(global, ERR_CYLINDER); // Comprobar si tokens es nulo
	char **orientation_tokens = ft_split(*tokens, ',');
	if (!orientation_tokens)
		finish(global, ERR_CYLINDER); // Comprobar si orientation_tokens es nulo
	cylinder.orientation.x = roundingf(ft_atof(orientation_tokens[0]), 2);
	cylinder.orientation.y = roundingf(ft_atof(orientation_tokens[1]), 2);
	cylinder.orientation.z = roundingf(ft_atof(orientation_tokens[2]), 2);
	free(orientation_tokens);

	// Parsear radio
	tokens++;
	if (!*tokens)
		finish(global, ERR_CYLINDER); // Comprobar si tokens es nulo
	cylinder.radius = roundingf(ft_atof(*tokens), 2);

	// Parsear altura
	tokens++;
	if (!*tokens)
		finish(global, ERR_CYLINDER); // Comprobar si tokens es nulo
	cylinder.height = roundingf(ft_atof(*tokens), 2);

	// Parsear color
	tokens++;
	if (!*tokens)
		finish(global, ERR_CYLINDER); // Comprobar si tokens es nulo
	char **color_tokens = ft_split(*tokens, ',');
	if (!color_tokens)
		finish(global, ERR_CYLINDER); // Comprobar si color_tokens es nulo
	cylinder.color.r = ft_atoi(color_tokens[0]);
	cylinder.color.g = ft_atoi(color_tokens[1]);
	cylinder.color.b = ft_atoi(color_tokens[2]);
	free(color_tokens);

	scene->cylinders[scene->num_cylinders++] = cylinder;
}
void check_scene(t_global *global, t_scene *scene)
{
	if (scene->ambient.initialized == 0 || scene->camera.initialized == 0 ||
		scene->light.initialized == 0)
		finish(global, ERR_SCENE);
	if (scene->num_spheres < 0 || scene->num_spheres > MAX_SPHERES)
		finish(global, ERR_SPHERE);
	if (scene->num_planes < 0 || scene->num_planes > MAX_PLANES)
		finish(global, ERR_PLANE);
	if (scene->num_cylinders < 0 || scene->num_cylinders > MAX_CYLINDERS)
		finish(global, ERR_CYLINDER);
}
// Function to read and parse the scene file
void read_scene(t_global *global)
{
	char *line_ptr;
	int fd;
	t_scene *scene;
	char *filename;

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
