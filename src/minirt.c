/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:20:25 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 22:46:05 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

// Function to check the file extension
void	check_file_extension(t_global *global, const char *filename)
{
	const char	*ext;

	ext = ft_strrchr(filename, '.');
	if (ft_strcmp(ext, ".rt") != 0)
		finish(global, ERR_INVALID_EXT);
	else if (!ext)
		finish(global, ERR_ARGS);
}

static t_global	*initialize_global(int argc, char **argv)
{
	t_global	*global;

	if (argc != 2)
		finish(NULL, ERR_ARGS);
	global = (t_global *)malloc(sizeof(t_global));
	if (!global)
		finish(NULL, ERR_MALLOC);
	memset(global, 0, sizeof(t_global));
	check_file_extension(global, argv[1]);
	init(global);
	init_scene(global);
	global->scene.file_path = argv[1];
	printf("Opening file: %s\n", global->scene.file_path);
	read_scene(global);
	return (global);
}

static void	setup_render_window(t_global *global)
{
	char	*info_str;
	char	*suffix;

	suffix = "    By anavas-g";
	global->vars.mlx_win = new_window(global);
	if (!global->vars.mlx_win)
		finish(global, ERR_WIN);
	render(global);
	mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
		global->img.img, MARGIN / 2, MARGIN / 2);
	info_str = ft_strjoin(global->scene.file_path, suffix);
	if (!info_str)
		finish(global, ERR_MEM);
	mlx_string_put(global->vars.mlx_conn, global->vars.mlx_win, WIN_W * .26,
		WIN_H * .98, MAGENTA, info_str);
	free(info_str);
}

int	main(int argc, char **argv)
{
	t_global	*global;

	global = initialize_global(argc, argv);
	setup_render_window(global);
	printf("Rendering complete. Entering main loop...\n");
	set_hooks(global);
	mlx_loop(global->vars.mlx_conn);
	return (SUCCESS);
}
