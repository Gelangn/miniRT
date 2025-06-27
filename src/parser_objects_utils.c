/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_objects_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:12:48 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/27 16:26:47 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Helper to parse tokens into a vector
static void	parse_tokens_to_vector(char **tokens, t_vector *vector)
{
	vector->x = ft_atof(tokens[0]);
	vector->y = ft_atof(tokens[1]);
	vector->z = ft_atof(tokens[2]);
}

// Helper to parse tokens into a color
static void	parse_tokens_to_color(char **tokens, t_color *color)
{
	color->r = ft_atoi(tokens[0]);
	color->g = ft_atoi(tokens[1]);
	color->b = ft_atoi(tokens[2]);
	if (color->r < 0 || color->r > 255 || color->g < 0 || color->g > 255
		|| color->b < 0 || color->b > 255)
		finish(NULL, ERR_COLOR);
}

// Function to parse vector
void	parse_vector(char *token, t_vector *vector)
{
	char	**tokens;

	tokens = ft_split(token, ',');
	if (!tokens || !tokens[0] || !tokens[1] || !tokens[2])
	{
		dbl_free(tokens);
		free(token);
		finish(NULL, ERR_PARSE);
	}
	parse_tokens_to_vector(tokens, vector);
	dbl_free(tokens);
}

// Function to parse color
void	parse_color(char *token, t_color *color)
{
	char	**tokens;

	tokens = ft_split(token, ',');
	if (!tokens || !tokens[0] || !tokens[1] || !tokens[2])
	{
		dbl_free(tokens);
		free(token);
		finish(NULL, ERR_PARSE);
	}
	parse_tokens_to_color(tokens, color);
	dbl_free(tokens);
}

