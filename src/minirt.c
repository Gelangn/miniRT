// incluir cabecera 42

#include "../inc/minirt.h"
#include <linux/limits.h>

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

int	main(int argc, char **argv)
{
	t_global	*global;
	char		buffer[PATH_MAX + 20];

	// Inicializa global a cero para evitar punteros no válidos
	if (argc != 2)
		finish(NULL, ERR_ARGS);
	global = (t_global *)malloc(sizeof(t_global));
	if (!global)
		finish(NULL, ERR_MALLOC);
	memset(global, 0, sizeof(t_global));
	check_file_extension(global, argv[1]);
	if ((init(global) != 0) || (init_scene(global, &global->scene) != 0))
		finish(global, ERR_MEM);
	global->scene.file_path = argv[1];
	printf("Opening file: %s\n", global->scene.file_path);
	read_scene(global);
	global->vars.mlx_win = new_window(global); // Create a new window
	if (!global->vars.mlx_win)
		finish(global, ERR_WIN);
	render(global);
	save_bmp(&global->img, WIN_W - MARGIN, WIN_H - MARGIN,
		"./imgs/ray_tracing.bmp", global);
	mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
		global->img.img, MARGIN / 2, MARGIN / 2);
	sprintf(buffer, "%s    By anavas-g", global->scene.file_path);
	mlx_string_put(global->vars.mlx_conn, global->vars.mlx_win, WIN_W * .26,
		WIN_H * .98, MAGENTA, buffer);
	set_hooks(global);
	printf("Rendering complete. Entering main loop...\n");
	mlx_loop(global->vars.mlx_conn);
	return (SUCCESS);
}
