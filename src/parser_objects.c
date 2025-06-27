/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/27 10:52:39 by anavas-g         ###   ########.fr       */
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
	sphere.material.trans = parse_float_token(global, tokens);
	tokens++;
	sphere.material.refl = parse_float_token(global, tokens);
	tokens++;
	sphere.material.refr_idx = parse_float_token(global, tokens);
	global->scene.spheres[global->scene.num_sp++] = sphere;
	dbl_free(tokens_start);
}

// Function to parse a plane
void	parse_plane(t_global *global, char *line)
{
	t_plane	plane;
	char	**tokens;
	t_scene	*scene;

	char **tokens_start;
	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	tokens_start = tokens;
	if (!tokens)
		finish(global, ERR_PLANE);
	tokens++;
	parse_vector(global, *tokens, &plane.point);
	tokens++;
	parse_vector(global, *tokens, &plane.normal);
	tokens++;
	parse_color(global, *tokens, &plane.material.color);
	tokens++;
	if (tokens && *tokens && *(tokens + 1) && *(tokens + 2))
	{
		plane.material.trans = parse_float_token(global, tokens);
		tokens++;
		plane.material.refl = parse_float_token(global, tokens);
		tokens++;
		plane.material.refr_idx = parse_float_token(global, tokens);
		tokens++;
	}
	else
	{
		plane.material.trans = 0.0f;
		plane.material.refl = 0.0f;
		plane.material.refr_idx = 1.5f;
	}
	scene->planes[scene->num_pl++] = plane;
	dbl_free(tokens_start);
}

// Function to parse a cylinder
void	parse_cylinder(t_global *global, char *line)
{
	t_cylinder	cyl;
	char		**tokens;
	t_scene		*scene;
	t_vector	center;

	char **tokens_start;
	scene = &global->scene;
	replace_tabs_with_spaces(line);
	tokens = ft_split(line, ' ');
	tokens_start = tokens;
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
	parse_color(global, *tokens, &cyl.material.color);
	tokens++;
	if (tokens && *tokens && *(tokens + 1) && *(tokens + 2))
	{
		cyl.material.trans = parse_float_token(global, tokens);
		tokens++;
		cyl.material.refl = parse_float_token(global, tokens);
		tokens++;
		cyl.material.refr_idx = parse_float_token(global, tokens);
		tokens++;
		printf("CILINDRO: Leyendo propiedades opcionales: transp=%.2f, \
				refl=%.2f, idx=%.2f\n",
				cyl.material.trans,
				cyl.material.refl,
				cyl.material.refr_idx);
	}
	else
	{
		cyl.material.trans = 0.0f;
		cyl.material.refl = 0.0f;
		cyl.material.refr_idx = 1.5f;
	}
	cyl.base = subtract(center, multiply(cyl.axis, cyl.height / 2));
	scene->cyls[scene->num_cy++] = cyl;
	dbl_free(tokens_start);
}
