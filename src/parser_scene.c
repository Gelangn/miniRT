/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/23 16:45:55 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/**
 * Parses ambient lighting information from a scene file line
 * Extracts intensity and color values for the global ambient illumination
 * Ensures ambient lighting is defined only once in the scene
 * 
 * @param global Structure containing scene data and parsing state
 * @param line String containing ambient light definition 
 * 				(format: "A intensity R,G,B")
 */
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

/**
 * Parses camera definition from a scene file line
 * Sets up camera position, orientation vector, and field of view
 * Validates that camera is only defined once per scene
 * 
 * @param global Structure containing scene data and parsing state
 * @param line String containing camera definition (format: "C pos ori fov")
 */
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
	parse_vector(global, *tokens, &scene->cam.dir);
	tokens++;
	scene->cam.fov = parse_int_token(global, tokens);
	scene->cam.init = 1;
	tokens -= 3;
	dbl_free(tokens);
}

/**
 * Parses light source information from a scene file line
 * Extracts position and brightness intensity for the scene's light source
 * Ensures light source is defined only once in the scene
 * 
 * @param global Structure containing scene data and parsing state
 * @param line String containing light definition (format: "L pos intensity")
 */
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
