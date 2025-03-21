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
	return ft_atof(*token);
}
// Function to split a string into tokens and convert to int
int parse_int(char **str)
{
	char **token = ft_split(*str, ',');
	return (ft_atoi(*token));
} */
// Función para reemplazar tabuladores por espacios
void replace_tabs_with_spaces(char *str)
{
	while (*str)
	{
		if (*str == '\t')
			*str = ' ';
		str++;
	}
}
// Function to parse ambient light
void parse_ambient(t_scene *scene, char *line)
{
	char **token;
	char **color_tokens;

	replace_tabs_with_spaces(line); // Reemplazar tabuladores por espacios
	token = ft_split(line, ' ');
	token++; // skip the first token

	// Parse intensity
	scene->ambient.intensity = ft_atof(*token);
	if (scene->ambient.intensity < 0.0 || scene->ambient.intensity > 1.0)
		finish(ERR_AMBIENT);

	// Parse color
	token++;
	color_tokens = ft_split(*token, ',');
	scene->ambient.color.r = ft_atoi(color_tokens[0]);
	scene->ambient.color.g = ft_atoi(color_tokens[1]);
	scene->ambient.color.b = ft_atoi(color_tokens[2]);

	// Free allocated memory for tokens
	free(color_tokens);
}
// Function to parse camera
void parse_camera(t_scene *scene, char *line)
{
	replace_tabs_with_spaces(line); // Reemplazar tabuladores por espacios
	char **tokens = ft_split(line, ' ');
	if (!tokens)
		finish(ERR_CAMERA); // Comprobar si tokens es nulo
	tokens++;				// saltar el primer token

	// Parsear posición
	char **position_tokens = ft_split(*tokens, ',');
	if (!position_tokens)
		finish(ERR_CAMERA); // Comprobar si position_tokens es nulo
	scene->camera.position.x = ft_atof(position_tokens[0]);
	scene->camera.position.y = ft_atof(position_tokens[1]);
	scene->camera.position.z = ft_atof(position_tokens[2]);
	free(position_tokens);

	// Parsear orientación
	tokens++;
	char **orientation_tokens = ft_split(*tokens, ',');
	if (!orientation_tokens)
		finish(ERR_CAMERA); // Comprobar si orientation_tokens es nulo
	scene->camera.orientation.x = ft_atof(orientation_tokens[0]);
	scene->camera.orientation.y = ft_atof(orientation_tokens[1]);
	scene->camera.orientation.z = ft_atof(orientation_tokens[2]);
	free(orientation_tokens);

	// Parsear FOV
	tokens++;
	if (!*tokens)
		finish(ERR_CAMERA); // Comprobar si tokens es nulo
	scene->camera.fov = ft_atoi(*tokens);
}
/* void parse_camera(t_scene *scene, char *line)
{
	char *str = line + 2;
	scene->camera.position.x = parse_float(&str);
	scene->camera.position.y = parse_float(&str);
	scene->camera.position.z = parse_float(&str);
	scene->camera.orientation.x = parse_float(&str);
	scene->camera.orientation.y = parse_float(&str);
	scene->camera.orientation.z = parse_float(&str);
	scene->camera.fov = parse_int(&str);
} */

// Function to parse light
void parse_light(t_scene *scene, char *line)
{
	replace_tabs_with_spaces(line); // Reemplazar tabuladores por espacios
	char **tokens = ft_split(line, ' ');
	tokens++; // saltar el primer token

	// Parsear posición
	char **position_tokens = ft_split(*tokens, ',');
	scene->light.position.x = ft_atof(position_tokens[0]);
	scene->light.position.y = ft_atof(position_tokens[1]);
	scene->light.position.z = ft_atof(position_tokens[2]);
	free(position_tokens);

	// Parsear intensidad
	tokens++;
	scene->light.intensity = ft_atof(*tokens);
}
/* void parse_light(t_scene *scene, char *line)
{
	char *str = line + 2;
	scene->light.position.x = parse_float(&str);
	scene->light.position.y = parse_float(&str);
	scene->light.position.z = parse_float(&str);
	scene->light.intensity = parse_float(&str);
} */

// Function to parse sphere
void parse_sphere(t_scene *scene, char *line)
{
	t_sphere sphere;
	replace_tabs_with_spaces(line); // Reemplazar tabuladores por espacios
	char **tokens = ft_split(line, ' ');
	tokens++; // saltar el primer token

	// Parsear centro
	char **center_tokens = ft_split(*tokens, ',');
	sphere.center.x = ft_atof(center_tokens[0]);
	sphere.center.y = ft_atof(center_tokens[1]);
	sphere.center.z = ft_atof(center_tokens[2]);
	free(center_tokens);

	// Parsear radio
	tokens++;
	sphere.radius = ft_atof(*tokens);

	// Parsear color
	tokens++;
	char **color_tokens = ft_split(*tokens, ',');
	sphere.color.r = ft_atoi(color_tokens[0]);
	sphere.color.g = ft_atoi(color_tokens[1]);
	sphere.color.b = ft_atoi(color_tokens[2]);
	free(color_tokens);

	scene->spheres[scene->num_spheres++] = sphere;
}
/* void parse_sphere(t_scene *scene, char *line)
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
} */

// Function to parse plane
void parse_plane(t_scene *scene, char *line)
{
	t_plane plane;
	replace_tabs_with_spaces(line); // Reemplazar tabuladores por espacios
	char **tokens = ft_split(line, ' ');
	if (!tokens)
		finish(ERR_PLANE); // Comprobar si tokens es nulo
	tokens++;			   // saltar el primer token

	// Parsear punto
	char **point_tokens = ft_split(*tokens, ',');
	if (!point_tokens)
		finish(ERR_PLANE); // Comprobar si point_tokens es nulo
	plane.point.x = ft_atof(point_tokens[0]);
	plane.point.y = ft_atof(point_tokens[1]);
	plane.point.z = ft_atof(point_tokens[2]);
	free(point_tokens);

	// Parsear normal
	tokens++;
	if (!*tokens)
		finish(ERR_PLANE); // Comprobar si tokens es nulo
	char **normal_tokens = ft_split(*tokens, ',');
	if (!normal_tokens)
		finish(ERR_PLANE); // Comprobar si normal_tokens es nulo
	plane.normal.x = ft_atof(normal_tokens[0]);
	plane.normal.y = ft_atof(normal_tokens[1]);
	plane.normal.z = ft_atof(normal_tokens[2]);
	free(normal_tokens);

	// Parsear color
	tokens++;
	if (!*tokens)
		finish(ERR_PLANE); // Comprobar si tokens es nulo
	char **color_tokens = ft_split(*tokens, ',');
	if (!color_tokens)
		finish(ERR_PLANE); // Comprobar si color_tokens es nulo
	plane.color.r = ft_atoi(color_tokens[0]);
	plane.color.g = ft_atoi(color_tokens[1]);
	plane.color.b = ft_atoi(color_tokens[2]);
	free(color_tokens);

	if (!scene)
	{
		printf("Error: scene es nulo\n");
		finish(ERR_PLANE);
	}
	if (!scene->planes)
	{
		printf("Error: scene->planes es nulo\n");
		finish(ERR_PLANE);
	}
	if (scene->num_planes >= MAX_PLANES)
	{ // Suponiendo que hay un límite MAX_PLANES
		printf("Error: num_planes excede el límite\n");
		finish(ERR_PLANE);
	}

	scene->planes[scene->num_planes++] = plane;
}
/* void parse_plane(t_scene *scene, char *line)
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
}*/
// Function to parse cylinder
void parse_cylinder(t_scene *scene, char *line)
{
	t_cylinder cylinder;
	replace_tabs_with_spaces(line); // Reemplazar tabuladores por espacios
	char **tokens = ft_split(line, ' ');
	tokens++; // saltar el primer token

	// Parsear base
	char **base_tokens = ft_split(*tokens, ',');
	cylinder.base.x = ft_atof(base_tokens[0]);
	cylinder.base.y = ft_atof(base_tokens[1]);
	cylinder.base.z = ft_atof(base_tokens[2]);
	free(base_tokens);

	// Parsear orientación
	tokens++;
	char **orientation_tokens = ft_split(*tokens, ',');
	cylinder.orientation.x = ft_atof(orientation_tokens[0]);
	cylinder.orientation.y = ft_atof(orientation_tokens[1]);
	cylinder.orientation.z = ft_atof(orientation_tokens[2]);
	free(orientation_tokens);

	// Parsear radio
	tokens++;
	cylinder.radius = ft_atof(*tokens);

	// Parsear altura
	tokens++;
	cylinder.height = ft_atof(*tokens);

	// Parsear color
	tokens++;
	char **color_tokens = ft_split(*tokens, ',');
	cylinder.color.r = ft_atoi(color_tokens[0]);
	cylinder.color.g = ft_atoi(color_tokens[1]);
	cylinder.color.b = ft_atoi(color_tokens[2]);
	free(color_tokens);

	scene->cylinders[scene->num_cylinders++] = cylinder;
}
/* void parse_cylinder(t_scene *scene, char *line)
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
} */

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
