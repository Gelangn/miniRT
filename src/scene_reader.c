/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:26:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/19 13:38:58 by anavas-g         ###   ########.fr       */
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
	else if (ft_strncmp(line, "sp", 2) == 0)
		parse_sphere(global, line);
	else if (ft_strncmp(line, "pl", 2) == 0)
		parse_plane(global, line);
	else if (ft_strncmp(line, "cy", 2) == 0)
		parse_cylinder(global, line);
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
	printf("line_ptr: %s\n", line_ptr);
	if (!line_ptr)
		finish(global, ERR_READ);
 	while (line_ptr)
	{
		write(1, line_ptr, ft_strlen(line_ptr));
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

	fd = open(global->scene.file_path, O_RDONLY);
	if (fd == -1)
		finish(global, ERR_OPEN);
	line_ptr = get_next_line(fd);
	while (line_ptr)
	{
		if (ft_strncmp(line_ptr, "sp", 2) == 0)
			global->scene.num_sp++;
		else if (ft_strncmp(line_ptr, "pl", 2) == 0)
			global->scene.num_pl++;
		else if (ft_strncmp(line_ptr, "cy", 2) == 0)
			global->scene.num_cy++;
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
