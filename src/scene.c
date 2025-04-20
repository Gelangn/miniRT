/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:26:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 18:24:16 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	check_scene(t_global *global, t_scene *scene)
{
	if (scene->ambient.init == 0 || scene->cam.init == 0
		|| scene->light.init == 0)
		finish(global, ERR_SCENE);
	if (scene->num_sp > MAX_SPHERES)
		finish(global, ERR_SPHERE);
	if (scene->num_pl > MAX_PLANES)
		finish(global, ERR_PLANE);
	if (scene->num_cy > MAX_CYLINDERS)
		finish(global, ERR_CYLINDER);
	if (scene->num_sp == 0 && scene->num_pl == 0 && scene->num_cy == 0)
		finish(global, ERR_SCENE);
}

/* Helper function to process each line according to its type */
static void	parse_line(t_global *global, char *line)
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
