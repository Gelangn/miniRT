/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 17:12:58 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Function to replace tabs with spaces
static void	replace_tabs_with_spaces(char *str)
{
	while (*str)
	{
		if (*str == '\t')
			*str = ' ';
		str++;
	}
}

// Function to parse vector
static void	parse_vector(t_global *global, char *str, t_vector *vector)
{
	char	**tokens;

	tokens = ft_split(str, ',');
	if (!tokens)
		finish(global, ERR_PARSE);
	vector->x = ft_atof(tokens[0]);
	vector->y = ft_atof(tokens[1]);
	vector->z = ft_atof(tokens[2]);
	dbl_free(tokens);
}

// Function to parse color
static void	parse_color(t_global *global, char *str, t_color *color)
{
	char	**tokens;

	tokens = ft_split(str, ',');
	if (!tokens)
		finish(global, ERR_PARSE);
	color->r = ft_atoi(tokens[0]);
	color->g = ft_atoi(tokens[1]);
	color->b = ft_atoi(tokens[2]);
	dbl_free(tokens);
}

// Function to parse a float token
static float	parse_float_token(t_global *global, char **tokens)
{
	if (!*tokens)
		finish(global, ERR_PARSE);
	return (ft_atof(*tokens));
}

// Function to parse a integer token
static int	parse_int_token(t_global *global, char **tokens)
{
	if (!*tokens)
		finish(global, ERR_PARSE);
	return (ft_atoi(*tokens));
}

// Function to parse ambient light
void	parse_ambient(t_global *global, char *line)
{
	char	**tokens;
	t_scene	*scene;

	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	if (!tokens || scene->ambient.init)
		finish(global, ERR_AMBIENT);
	tokens++;
	scene->ambient.intensity = parse_float_token(global, tokens);
	tokens++;
	parse_color(global, *tokens, &scene->ambient.color);
	scene->ambient.init = 1;
	tokens -= 2;
	dbl_free(tokens);
}

// Function to parse camera
void	parse_cam(t_global *global, char *line)
{
	char	**tokens;
	t_scene	*scene;

	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	if (!tokens || scene->cam.init)
		finish(global, ERR_CAMERA);
	tokens++;
	parse_vector(global, *tokens, &scene->cam.pos);
	tokens++;
	parse_vector(global, *tokens, &scene->cam.orientation);
	tokens++;
	scene->cam.fov = parse_int_token(global, tokens);
	scene->cam.init = 1;
	tokens -= 3;
	dbl_free(tokens);
}

// Function to parse light
void	parse_light(t_global *global, char *line)
{
	char	**tokens;
	t_scene	*scene;

	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	if (!tokens || scene->light.init)
		finish(global, ERR_LIGHT);
	tokens++;
	parse_vector(global, *tokens, &scene->light.pos);
	tokens++;
	scene->light.intensity = parse_float_token(global, tokens);
	scene->light.init = 1;
	tokens -= 2;
	dbl_free(tokens);
}

// Function to parse a sphere
void	parse_sphere(t_global *global, char *line)
{
	t_sphere	sphere;
	char		**tokens;
	t_scene		*scene;

	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	if (!tokens)
		finish(global, ERR_SPHERE);
	tokens++;
	parse_vector(global, *tokens, &sphere.center);
	tokens++;
	sphere.radius = parse_float_token(global, tokens) / 2;
	// Recibimos el diámetro
	tokens++;
	parse_color(global, *tokens, &sphere.color);
	scene->spheres[scene->num_sp++] = sphere;
	tokens -= 3;
	dbl_free(tokens);
}

// Function to parse a plane
void	parse_plane(t_global *global, char *line)
{
	t_plane	plane;
	char	**tokens;
	t_scene	*scene;

	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	if (!tokens)
		finish(global, ERR_PLANE);
	tokens++;
	parse_vector(global, *tokens, &plane.point);
	tokens++;
	parse_vector(global, *tokens, &plane.normal);
	tokens++;
	parse_color(global, *tokens, &plane.color);
	scene->planes[scene->num_pl++] = plane;
	tokens -= 3;
	dbl_free(tokens);
}

// Function to parse a cylinder
void	parse_cylinder(t_global *global, char *line)
{
	t_cylinder	cylinder;
	char		**tokens;
	t_scene		*scene;

	scene = &global->scene;
	t_vector center; // Centro de masa temporal
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	if (!tokens)
		finish(global, ERR_CYLINDER);
	tokens++;
	// Parsear la posición como el centro del cilindro
	parse_vector(global, *tokens, &center);
	tokens++;
	parse_vector(global, *tokens, &cylinder.orientation);
	// Normalizar inmediatamente la orientación
	cylinder.orientation = normalize(cylinder.orientation);
	tokens++;
	cylinder.radius = parse_float_token(global, tokens) / 2;
	tokens++;
	cylinder.height = parse_float_token(global, tokens);
	tokens++;
	parse_color(global, *tokens, &cylinder.color);
	// Calcular la base desde el centro
	// La base es el punto inferior del cilindro en la dirección del eje
	cylinder.base = subtract(center, multiply(cylinder.orientation,
				cylinder.height / 2));
	scene->cylinders[scene->num_cy++] = cylinder;
	tokens -= 5;
	dbl_free(tokens);
}
