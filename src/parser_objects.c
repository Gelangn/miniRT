/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/21 14:24:55 by anavas-g         ###   ########.fr       */
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

	char **tokens_start; // Guardar puntero original
	tokens = ft_split(line, ' ');
	tokens_start = tokens; // Guardar puntero original
	if (!tokens)
		finish(global, ERR_MEM);
	sphere = &global->scene.spheres[global->scene.num_sp];
	// Parse sphere parameters with error checking
	if (!tokens[1] || !tokens[2] || !tokens[3])
	{
		dbl_free(tokens_start); // Usar puntero original
		finish(global, "Error: Invalid sphere format");
	}
	parse_vector(global, tokens[1], &sphere->center);
	sphere->radius = parse_float_token(global, &tokens[2]) / 2.0f;
	parse_color(global, tokens[3], &sphere->color);
	// Parse optional material properties usando índice 4, 5, 6
	if (tokens[4] && tokens[5] && tokens[6])
	{
		sphere->transp = parse_float_token(global, &tokens[4]);
		sphere->reflct = parse_float_token(global, &tokens[5]);
		sphere->refractive_idx = parse_float_token(global, &tokens[6]);
	}
	else
	{
		// Default values for backward compatibility
		sphere->transp = 0.0f;
		sphere->reflct = 0.0f;
		sphere->refractive_idx = 1.5f;
	}
	if (sphere->radius <= 0)
	{
		dbl_free(tokens_start); // Usar puntero original
		finish(global, "Error: Invalid sphere radius");
	}
	global->scene.num_sp++;
	dbl_free(tokens_start); // Usar puntero original
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
	parse_color(global, *tokens, &plane.color);
	tokens++;
	// Parse optional material properties
	if (tokens && *tokens && *(tokens + 1) && *(tokens + 2))
	{
		plane.transp = parse_float_token(global, tokens);
		tokens++;
		plane.reflct = parse_float_token(global, tokens);
		tokens++;
		plane.refractive_idx = parse_float_token(global, tokens);
		tokens++;
	}
	else
	{
		// Default values
		plane.transp = 0.0f;
		plane.reflct = 0.0f;
		plane.refractive_idx = 1.5f;
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
	parse_color(global, *tokens, &cyl.color);
	tokens++;
	// Parse optional material properties
	if (tokens && *tokens && *(tokens + 1) && *(tokens + 2))
	{
		cyl.transp = parse_float_token(global, tokens);
		tokens++;
		cyl.reflct = parse_float_token(global, tokens);
		tokens++;
		cyl.refractive_idx = parse_float_token(global, tokens);
		tokens++;
		printf("CILINDRO: Leyendo propiedades opcionales: transp=%.2f, \
				refl=%.2f, idx=%.2f\n",
				cyl.transp,
				cyl.reflct,
				cyl.refractive_idx);
	}
	else
	{
		// Default values
		cyl.transp = 0.0f;
		cyl.reflct = 0.0f;
		cyl.refractive_idx = 1.5f;
	}
	cyl.base = subtract(center, multiply(cyl.axis, cyl.height / 2));
	scene->cyls[scene->num_cy++] = cyl;
	// Usar siempre el puntero original para liberar
	dbl_free(tokens_start);
}
