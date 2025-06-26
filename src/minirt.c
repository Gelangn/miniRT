/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:20:25 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/26 11:28:42 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	init(t_global *global)
{
	global->img.img = NULL;
	global->img.addr = NULL;
	global->scene = (t_scene){0};
	global->total_pixels = (WIN_W - MARGIN) * (WIN_H - MARGIN);
}

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

static t_global	*initialize_global(t_global *global, int argc, char **argv)
{
	if (argc != 2)
		finish(NULL, ERR_ARGS);
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

	suffix = " By anavas-g & bde-mada";
	printf("Initializing MLX\n");
	global->vars.mlx_conn = mlx_init();
	if (!global->vars.mlx_conn)
		finish(global, ERR_MLX);
	printf("Initialized MLX OK\n");
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
	t_global	global;

	initialize_global(&global, argc, argv);
	setup_render_window(&global);
	printf("Rendering complete. Entering main loop...\n");
	set_hooks(&global);
	mlx_loop(global.vars.mlx_conn);
	return (SUCCESS);
}
