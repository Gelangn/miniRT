/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/26 14:14:14 by anavas-g         ###   ########.fr       */
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
	char		**tokens_start;

	tokens = ft_split(line, ' ');
	tokens_start = tokens;
	if (!tokens)
		finish(global, ERR_SPHERE);
	tokens++;
	parse_vector(global, *tokens, &sphere.center);
	tokens++;
	sphere.radius = parse_float_token(global, tokens);
	tokens++;
	parse_color(global, *tokens, &sphere.material.color);
	tokens++;
	sphere.material.transparency = parse_float_token(global, tokens);
	tokens++;
	sphere.material.reflectivity = parse_float_token(global, tokens);
	tokens++;
	sphere.material.refractive_idx = parse_float_token(global, tokens);
	global->scene.spheres[global->scene.num_sp++] = sphere;
	dbl_free(tokens_start);
}

// Function to parse a plane
void	parse_plane(t_global *global, char *line)
{
	t_plane	plane;
	char	**tokens;
	t_scene	*scene;

	char **tokens_start; // Guardar puntero original
	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	tokens_start = tokens; // Guardar puntero al inicio
	if (!tokens)
		finish(global, ERR_PLANE);
	tokens++;
	parse_vector(global, *tokens, &plane.point);
	tokens++;
	parse_vector(global, *tokens, &plane.normal);
	tokens++;
	parse_color(global, *tokens, &plane.material.color); // Usar material.color
	tokens++;
	// Parse optional material properties
	if (tokens && *tokens && *(tokens + 1) && *(tokens + 2))
	{
		plane.material.transparency = parse_float_token(global, tokens);
			// Usar material.transparency
		tokens++;
		plane.material.reflectivity = parse_float_token(global, tokens);
			// Usar material.reflectivity
		tokens++;
		plane.material.refractive_idx = parse_float_token(global, tokens);
			// Usar material.refractive_idx
		tokens++;
	}
	else
	{
		// Default values
		plane.material.transparency = 0.0f;
		plane.material.reflectivity = 0.0f;
		plane.material.refractive_idx = 1.5f;
	}
	scene->planes[scene->num_pl++] = plane;
	// Usar siempre el puntero original para liberar
	dbl_free(tokens_start);
}

// Function to parse a cylinder
void	parse_cylinder(t_global *global, char *line)
{
	t_cylinder	cyl;
	char		**tokens;
	t_scene		*scene;
	t_vector	center;

	char **tokens_start; // Guardar puntero original
	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	tokens_start = tokens; // Guardar puntero al inicio
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
	parse_color(global, *tokens, &cyl.material.color); // Usar material.color
	tokens++;
	// Parse optional material properties
	if (tokens && *tokens && *(tokens + 1) && *(tokens + 2))
	{
		cyl.material.transparency = parse_float_token(global, tokens);
			// Usar material.transparency
		tokens++;
		cyl.material.reflectivity = parse_float_token(global, tokens);
			// Usar material.reflectivity
		tokens++;
		cyl.material.refractive_idx = parse_float_token(global, tokens);
			// Usar material.refractive_idx
		tokens++;
		printf("CILINDRO: Leyendo propiedades opcionales: transp=%.2f, \
				refl=%.2f, idx=%.2f\n",
				cyl.material.transparency,
				cyl.material.reflectivity,
				cyl.material.refractive_idx);
	}
	else
	{
		// Default values
		cyl.material.transparency = 0.0f;
		cyl.material.reflectivity = 0.0f;
		cyl.material.refractive_idx = 1.5f;
	}
	cyl.base = subtract(center, multiply(cyl.axis, cyl.height / 2));
	scene->cyls[scene->num_cy++] = cyl;
	// Usar siempre el puntero original para liberar
	dbl_free(tokens_start);
}
