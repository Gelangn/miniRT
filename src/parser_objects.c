/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_objects.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/27 16:25:31 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Helper to parse material properties
static void	parse_material_properties(char **tokens, t_material *material)
{
	if (tokens && *tokens && *(tokens + 1) && *(tokens + 2))
	{
		material->trans = ft_atof(*tokens++);
		material->refl = ft_atof(*tokens++);
		material->refr_idx = ft_atof(*tokens++);
	}
	else
	{
		material->trans = 0.0f;
		material->refl = 0.0f;
		material->refr_idx = 1.5f;
	}
}

// Function to parse a sphere
void	parse_sp(t_global *global, char *ln)
{
	t_sphere	sphere;
	char		**tokens;
	char		**tokens_start;

	replace_tabs_with_spaces(ln);
	tokens = ft_split(ln, ' ');
	tokens_start = tokens;
	if (!tokens)
		finish(global, ERR_SPHERE);
	tokens++;
	parse_vector(*tokens++, &sphere.center);
	sphere.radius = ft_atof(*tokens++);
	parse_color(*tokens++, &sphere.material.color);
	parse_material_properties(tokens, &sphere.material);
	global->scene.spheres[global->scene.num_sp++] = sphere;
	dbl_free(tokens_start);
}

// Function to parse a plane
void	parse_pl(t_global *global, char *ln)
{
	t_plane	plane;
	char	**tokens;
	char	**tokens_start;

	replace_tabs_with_spaces(ln);
	tokens = ft_split(ln, ' ');
	tokens_start = tokens;
	if (!tokens)
		finish(global, ERR_PLANE);
	tokens++;
	parse_vector(*tokens++, &plane.point);
	parse_vector(*tokens++, &plane.normal);
	parse_color(*tokens++, &plane.material.color);
	parse_material_properties(tokens, &plane.material);
	global->scene.planes[global->scene.num_pl++] = plane;
	dbl_free(tokens_start);
}

// Function to parse a cylinder
void	parse_cyl(t_global *global, char *ln)
{
	t_cylinder	cyl;
	t_vector	center;
	char		**tokens;
	char		**tokens_start;

	replace_tabs_with_spaces(ln);
	tokens = ft_split(ln, ' ');
	tokens_start = tokens;
	if (!tokens)
		finish(global, ERR_CYLINDER);
	tokens++;
	parse_vector(*tokens++, &center);
	parse_vector(*tokens++, &cyl.axis);
	cyl.axis = norm(cyl.axis);
	cyl.radius = ft_atof(*tokens++) / 2;
	cyl.height = ft_atof(*tokens++);
	parse_color(*tokens++, &cyl.material.color);
	parse_material_properties(tokens, &cyl.material);
	cyl.base = subtract(center, multiply(cyl.axis, cyl.height / 2));
	global->scene.cyls[global->scene.num_cy++] = cyl;
	dbl_free(tokens_start);
}
