/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_scene.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/21 21:16:12 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

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
