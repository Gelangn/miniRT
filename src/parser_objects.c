/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-mada <bde-mada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/18 19:59:30 by bde-mada         ###   ########.fr       */
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
	t_sphere	sphere;
	char		**tokens;
	t_scene		*scene;

	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	if (!tokens)
		finish(global, ERR_SPHERE);
	write(1, "Parsing sphere\n", 15);
	tokens++;
	parse_vector(global, *tokens, &sphere.center);
	write(1, "Parsed center\n", 14);
	tokens++;
	sphere.radius = parse_float_token(global, tokens) / 2;
	write(1, "Parsed radius\n", 15);
	tokens++;
	parse_color(global, *tokens, &sphere.color);
	write(1, "Parsed color\n", 14);
	tokens++;
	
	// Parse optional transparency (default 0.0 = opaque)
	if (*tokens && is_valid_float(*tokens))
	{
		sphere.transparency = parse_float_token(global, tokens);
		sphere.transparency = fmax(0.0f, fmin(1.0f, sphere.transparency)); // Clamp 0-1
		tokens++;
	}
	else
		sphere.transparency = 0.0f;
	
	// Parse optional reflectivity (default 0.0 = no reflection)
	if (*tokens && is_valid_float(*tokens))
	{
		sphere.reflectivity = parse_float_token(global, tokens);
		sphere.reflectivity = fmax(0.0f, fmin(1.0f, sphere.reflectivity)); // Clamp 0-1
		tokens++;
	}
	else
		sphere.reflectivity = 0.0f;
	
	// Parse optional refractive index (default 1.5 = glass)
	if (*tokens && is_valid_float(*tokens))
	{
		sphere.refractive_index = parse_float_token(global, tokens);
		sphere.refractive_index = fmax(1.0f, sphere.refractive_index); // Must be >= 1.0
		tokens++;
	}
	else
		sphere.refractive_index = 1.5f;
	
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
    cyl.transparency = 0.0f;      // Default: opaque
    cyl.reflectivity = 0.0f;      // Default: no reflection
    cyl.refractive_index = 1.5f;  // Default: glass-like
    
	scene->cyls[scene->num_cy++] = cyl;
	tokens -= 5;
	dbl_free(tokens);
}
