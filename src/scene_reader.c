/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:26:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/26 11:31:55 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* Helper function to process each line according to its type */
void	parse_line(t_global *global, char *line)
{
	// Ignorar líneas vacías
	if (!line || !*line)
		return ;
		
	// Omitir espacios iniciales
	while (*line && (*line == ' ' || *line == '\t'))
		line++;
	
	// Ignorar comentarios o líneas vacías después de eliminar espacios
	if (!*line || *line == '#')
		return ;
		
	// Ahora analizar el comando
	if (*line == 'A' && (line[1] == ' ' || line[1] == '\t'))
		parse_ambient(global, line);
	else if (*line == 'C' && (line[1] == ' ' || line[1] == '\t'))
		parse_cam(global, line);
	else if (*line == 'L' && (line[1] == ' ' || line[1] == '\t'))
		parse_light(global, line);
	else if (ft_strncmp(line, "sp", 2) == 0 && (line[2] == ' '
				|| line[2] == '\t'))
		parse_sphere(global, line);
	else if (ft_strncmp(line, "pl", 2) == 0 && (line[2] == ' '
				|| line[2] == '\t'))
		parse_plane(global, line);
	else if (ft_strncmp(line, "cy", 2) == 0 && (line[2] == ' '
				|| line[2] == '\t'))
		parse_cylinder(global, line);
	else
	{
		free(line);
		finish(global, ERR_PARSE " - Invalid line format");
	}
}

/* Function to read and parse the scene file */
void	read_scene(t_global *global)
{
	char	*line_ptr;
	int		fd;

	count_objects(global);
	allocate_objects(global);
	fd = open(global->scene.file_path, O_RDONLY);
	if (fd == -1)
		finish(global, ERR_OPEN);
	line_ptr = get_next_line(fd);
	printf("line_ptr:\n %s\n", line_ptr);
	if (!line_ptr)
		finish(global, ERR_READ);
	while (line_ptr)
	{
		if (line_ptr[0] == '\n' || line_ptr[0] == '\0' || line_ptr[0] == '#')
		{
			free(line_ptr);
			line_ptr = get_next_line(fd);
			continue ;
		}
		parse_line(global, line_ptr);
		free(line_ptr);
		line_ptr = get_next_line(fd);
	}
	check_scene(global, &global->scene);
	close(fd);
}

void	count_objects(t_global *global)
{
	char	*line_ptr;
	int		fd;
	char	*trimmed_line;

	fd = open(global->scene.file_path, O_RDONLY);
	if (fd == -1)
		finish(global, ERR_OPEN);
	line_ptr = get_next_line(fd);
	while (line_ptr)
	{
		trimmed_line = line_ptr;
		while (*trimmed_line && (*trimmed_line == ' ' || *trimmed_line == '\t'))
			trimmed_line++;
		if (*trimmed_line && *trimmed_line != '#')
		{
			if (ft_strncmp(trimmed_line, "sp", 2) == 0)
				global->scene.num_sp++;
			else if (ft_strncmp(trimmed_line, "pl", 2) == 0)
				global->scene.num_pl++;
			else if (ft_strncmp(trimmed_line, "cy", 2) == 0)
				global->scene.num_cy++;
		}
		free(line_ptr);
		line_ptr = get_next_line(fd);
	}
	close(fd);
}

void	allocate_objects(t_global *global)
{
	t_scene	*scene;

	scene = &global->scene;
	if (scene->num_sp > MAX_SPHERES || scene->num_pl > MAX_PLANES
		|| scene->num_cy > MAX_CYLINDERS)
		finish(global, ERR_SCENE);
	scene->spheres = NULL;
	scene->planes = NULL;
	scene->cyls = NULL;
	if (scene->num_sp > 0)
		scene->spheres = malloc(sizeof(t_sphere) * scene->num_sp);
	if (scene->num_pl > 0)
		scene->planes = malloc(sizeof(t_plane) * scene->num_pl);
	if (scene->num_cy > 0)
		scene->cyls = malloc(sizeof(t_cylinder) * scene->num_cy);
	if ((scene->num_sp > 0 && !scene->spheres) || (scene->num_pl > 0
			&& !scene->planes) || (scene->num_cy > 0 && !scene->cyls))
		finish(global, ERR_MEM);
	scene->num_sp = 0;
	scene->num_pl = 0;
	scene->num_cy = 0;
}
