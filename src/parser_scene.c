/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/27 16:27:13 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Parses ambient lighting information from a scene file line
 * Extracts intensity and color values for the global ambient illumination
 * Ensures ambient lighting is defined only once in the scene
 * 
 * @param global Structure containing scene data and parsing state
 * @param ln String containing ambient light definition 
 * 				(format: "A intensity R,G,B")
 */
void	parse_ambient(t_global *global, char *ln)
{
	char	**tokens;
	t_scene	*scene;

	scene = &global->scene;
	replace_tabs_with_spaces(ln);
	tokens = ft_split(ln, ' ');
	if (!tokens || scene->ambient.init)
	{
		dbl_free(tokens);
		free(ln);
		finish(global, ERR_AMBIENT);
	}
	tokens++;
	scene->ambient.intens = parse_float_token(global, tokens);
	tokens++;
	parse_color(*tokens, &scene->ambient.color);
	scene->ambient.init = 1;
	tokens -= 2;
	dbl_free(tokens);
}

/**
 * Parses camera definition from a scene file ln
 * Sets up camera position, orientation vector, and field of view
 * Validates that camera is only defined once per scene
 * 
 * @param global Structure containing scene data and parsing state
 * @param ln String containing camera definition (format: "C pos ori fov")
 */
void	parse_cam(t_global *global, char *ln)
{
	char	**tokens;
	t_scene	*scene;

	scene = &global->scene;
	replace_tabs_with_spaces(ln);
	tokens = ft_split(ln, ' ');
	if (!tokens || scene->cam.init)
	{
		dbl_free(tokens);
		free(ln);
		finish(global, ERR_CAMERA);
	}
	tokens++;
	parse_vector(*tokens, &scene->cam.pos);
	tokens++;
	parse_vector(*tokens, &scene->cam.dir);
	tokens++;
	scene->cam.fov = parse_int_token(global, tokens);
	scene->cam.init = 1;
	tokens -= 3;
	dbl_free(tokens);
}

/**
 * Parses light source information from a scene file ln
 * Extracts position and brightness intensity for the scene's light source
 * Ensures light source is defined only once in the scene
 * 
 * @param global Structure containing scene data and parsing state
 * @param ln String containing light definition (format: "L pos intensity")
 */
void	parse_light(t_global *global, char *ln)
{
	char	**tokens;
	t_scene	*scene;

	scene = &global->scene;
	replace_tabs_with_spaces(ln);
	tokens = ft_split(ln, ' ');
	if (!tokens || scene->light.init)
		finish(global, ERR_LIGHT);
	tokens++;
	parse_vector(*tokens, &scene->light.pos);
	tokens++;
	scene->light.intens = parse_float_token(global, tokens);
	scene->light.init = 1;
	tokens -= 2;
	dbl_free(tokens);
}
