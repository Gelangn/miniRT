/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/26 10:39:36 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Function to parse vector
void	parse_vector(t_global *global, char *str, t_vector *vector)
{
	char	**tokens;

	tokens = ft_split(str, ',');
	if (!tokens || !tokens[0] || !tokens[1] || !tokens[2])
	{
		dbl_free(tokens);
		free(str);
		finish(global, ERR_PARSE);
	}
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
	if (!tokens || !tokens[0] || !tokens[1] || !tokens[2])
	{
		dbl_free(tokens);
		finish(global, ERR_PARSE);
	}
	color->r = ft_atoi(tokens[0]);
	color->g = ft_atoi(tokens[1]);
	color->b = ft_atoi(tokens[2]);
	if (color->r < 0 || color->r > 255 || color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
	{
		dbl_free(tokens);
		finish(global, ERR_COLOR);
	}
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
	parse_vector(global, *tokens, &cyl.orientation);
	cyl.orientation = norm(cyl.orientation);
	tokens++;
	cyl.radius = parse_float_token(global, tokens) / 2;
	tokens++;
	cyl.height = parse_float_token(global, tokens);
	tokens++;
	parse_color(global, *tokens, &cyl.color);
	cyl.base = subtract(center, multiply(cyl.orientation, cyl.height / 2));
	scene->cyls[scene->num_cy++] = cyl;
	tokens -= 5;
	dbl_free(tokens);
}
