/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 13:39:01 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Function to parse vector
void	parse_vector(t_global *global, char *str, t_vector *vector)
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
void	parse_color(t_global *global, char *str, t_color *color)
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

// Function to parse a sphere
void	parse_sphere(t_global *global, char *line)
{
	char		**tokens;
	t_sphere	*sphere;

	tokens = ft_split(line, ' ');
	if (!tokens)
		finish(global, ERR_MEM);
	sphere = &global->scene.spheres[global->scene.num_sp];
	// Parse sphere parameters with error checking
	if (!tokens[1] || !tokens[2] || !tokens[3])
	{
		dbl_free(tokens);
		finish(global, "Error: Invalid sphere format");
	}
	parse_vector(global, tokens[1], &sphere->center);
	sphere->radius = parse_float_token(global, &tokens[2]) / 2.0f;
	parse_color(global, tokens[3], &sphere->color);
	// Parse optional material properties
	if (tokens[4] && tokens[5] && tokens[6])
	{
		sphere->transparency = parse_float_token(global, &tokens[4]);
		sphere->reflectivity = parse_float_token(global, &tokens[5]);
		sphere->refractive_index = parse_float_token(global, &tokens[6]);
	}
	else
	{
		// Default values for backward compatibility
		sphere->transparency = 0.0f;
		sphere->reflectivity = 0.0f;
		sphere->refractive_index = 1.5f;
	}
	if (sphere->radius <= 0)
	{
		dbl_free(tokens);
		finish(global, "Error: Invalid sphere radius");
	}
	global->scene.num_sp++;
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
	// Initialize new material properties with default values
	plane.transparency = 0.0f;
	plane.reflectivity = 0.0f;
	plane.refractive_index = 1.5f;
	scene->planes[scene->num_pl++] = plane;
	tokens -= 3;
	dbl_free(tokens);
}

// Function to parse a cylinder
void	parse_cylinder(t_global *global, char *line)
{
	t_cylinder	cyl;
	char		**tokens;
	t_scene		*scene;
	t_vector	center;

	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	if (!tokens)
		finish(global, ERR_CYLINDER);
	tokens++;
	parse_vector(global, *tokens, &center);
	tokens++;
	parse_vector(global, *tokens, &cyl.axis);
	cyl.axis = norm(cyl.axis);
	tokens++;
	cyl.radius = parse_float_token(global, tokens) / 2;
	tokens++;
	cyl.height = parse_float_token(global, tokens);
	tokens++;
	parse_color(global, *tokens, &cyl.color);
	cyl.base = subtract(center, multiply(cyl.axis, cyl.height / 2));
	// Initialize new material properties with default values
	cyl.transparency = 0.0f;     // Default: opaque
	cyl.reflectivity = 0.0f;     // Default: no reflection
	cyl.refractive_index = 1.5f; // Default: glass-like
	scene->cyls[scene->num_cy++] = cyl;
	tokens -= 5;
	dbl_free(tokens);
}
