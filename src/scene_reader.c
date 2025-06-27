/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:26:44 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/27 15:47:51 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

/* Helper function to process each ln according to its type */
void	parse_ln(t_global *global, char *ln)
{
	if (!ln || !*ln)
		return ;
	while (*ln && (*ln == ' ' || *ln == '\t'))
		ln++;
	if (!*ln || *ln == '#')
		return ;
	if (*ln == 'A' && (ln[1] == ' ' || ln[1] == '\t'))
		parse_ambient(global, ln);
	else if (*ln == 'C' && (ln[1] == ' ' || ln[1] == '\t'))
		parse_cam(global, ln);
	else if (*ln == 'L' && (ln[1] == ' ' || ln[1] == '\t'))
		parse_light(global, ln);
	else if (ft_strncmp(ln, "sp", 2) == 0 && (ln[2] == ' ' || ln[2] == '\t'))
		parse_sp(global, ln);
	else if (ft_strncmp(ln, "pl", 2) == 0 && (ln[2] == ' ' || ln[2] == '\t'))
		parse_pl(global, ln);
	else if (ft_strncmp(ln, "cy", 2) == 0 && (ln[2] == ' ' || ln[2] == '\t'))
		parse_cyl(global, ln);
	else
	{
		free(ln);
		finish(global, ERR_PARSE " - Invalid ln format");
	}
}

static void	allocate_objects(t_global *global)
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

void	count_objects(t_global *global)
{
	char	*ln_ptr;
	int		fd;

	fd = open(global->scene.file_path, O_RDONLY);
	if (fd == -1)
		finish(global, ERR_OPEN);
	ln_ptr = get_next_line(fd);
	while (ln_ptr)
	{
		while (*ln_ptr && (*ln_ptr == ' ' || *ln_ptr == '\t'))
			ln_ptr++;
		if (*ln_ptr && *ln_ptr != '#')
		{
			if (ft_strncmp(ln_ptr, "sp", 2) == 0)
				global->scene.num_sp++;
			else if (ft_strncmp(ln_ptr, "pl", 2) == 0)
				global->scene.num_pl++;
			else if (ft_strncmp(ln_ptr, "cy", 2) == 0)
				global->scene.num_cy++;
		}
		free(ln_ptr);
		ln_ptr = get_next_line(fd);
	}
	close(fd);
}

/* Function to read and parse the scene file */
void	read_scene(t_global *global)
{
	char	*ln_ptr;
	int		fd;

	count_objects(global);
	allocate_objects(global);
	fd = open(global->scene.file_path, O_RDONLY);
	if (fd == -1)
		finish(global, ERR_OPEN);
	ln_ptr = get_next_line(fd);
	if (!ln_ptr)
		finish(global, ERR_READ);
	while (ln_ptr)
	{
		if (ln_ptr[0] == '\n' || ln_ptr[0] == '\0' || ln_ptr[0] == '#')
		{
			free(ln_ptr);
			ln_ptr = get_next_line(fd);
			continue ;
		}
		parse_ln(global, ln_ptr);
		free(ln_ptr);
		ln_ptr = get_next_line(fd);
	}
	check_scene(global, &global->scene);
	close(fd);
}
