/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_parser_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 21:59:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/05 22:01:14 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Function to replace tabs with spaces
void	replace_tabs_with_spaces(char *str)
{
	while (*str)
	{
		if (*str == '\t')
			*str = ' ';
		str++;
	}
}

// Function to parse a float token
float	parse_float_token(t_global *global, char **tokens)
{
	if (!*tokens)
		finish(global, ERR_PARSE);
	return (ft_atof(*tokens));
}

// Function to parse a integer token
int	parse_int_token(t_global *global, char **tokens)
{
	if (!*tokens)
		finish(global, ERR_PARSE);
	return (ft_atoi(*tokens));
}
