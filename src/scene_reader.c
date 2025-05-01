/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:26:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/05/01 23:00:29 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* Helper function to process each line according to its type */
void	parse_line(t_global *global, char *line)
{
	if (line[0] == 'A')
		parse_ambient(global, line);
	else if (line[0] == 'C')
		parse_cam(global, line);
	else if (line[0] == 'L')
		parse_light(global, line);
	else if (strncmp(line, "sp", 2) == 0)
		parse_sphere(global, line);
	else if (strncmp(line, "pl", 2) == 0)
		parse_plane(global, line);
	else if (strncmp(line, "cy", 2) == 0)
		parse_cylinder(global, line);
}

/* Function to read and parse the scene file */
void	read_scene(t_global *global)
{
	char	*line_ptr;
	int		fd;

	fd = open(global->scene.file_path, O_RDONLY);
	if (fd == -1)
		finish(global, ERR_OPEN);
	line_ptr = get_next_line(fd);
	if (!line_ptr)
		finish(global, ERR_READ);
	while (line_ptr)
	{
		parse_line(global, line_ptr);
		free(line_ptr);
		line_ptr = get_next_line(fd);
	}
	check_scene(global, &global->scene);
	close(fd);
}

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
