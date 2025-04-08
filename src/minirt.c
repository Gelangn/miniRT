// incluir cabecera 42

#include "../inc/minirt.h"
#include <linux/limits.h>

/* static void	background(t_img *data)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (x < (WIN_W - MARGIN))
	{
		y = 0;
		while (y < (WIN_H - MARGIN))
		{
			pixel_put(data, x, y, GROUND_COLOR);
			y++;
		}
		x++;
	}
} */
int	init(t_global *global)
{
	// global->scene.points = NULL;
	global->img.img = NULL;
	global->img.addr = NULL;
	global->scene = (t_scene){0};
	printf("Initializing MLX\n");
	global->vars.mlx_conn = mlx_init();
	if (!global->vars.mlx_conn)
	{
		perror("Error initializing MLX");
		return (MLX_ERROR);
	}
	printf("Initialized MLX OK\n");
	return (MLX_SUCCESS);
}

static int	init_scene(t_global *global, t_scene *scene)
{
	scene->file_path = NULL;
	scene->fd = 0;
	scene->lines = NULL;
	scene->scale = 0;
	scene->ambient = (t_ambient){0};
	scene->camera = (t_camera){0};
	scene->light = (t_light){0};
	scene->ambient.init = 0;
	scene->camera.init = 0;
	scene->light.init = 0;
	scene->spheres = malloc(sizeof(t_plane) * MAX_SPHERES);
	if (!scene->spheres)
		finish(global, ERR_MEM);
	scene->num_sp = 0;
	scene->planes = malloc(sizeof(t_plane) * MAX_PLANES);
	if (!scene->planes)
		finish(global, ERR_MEM);
	scene->num_pl = 0;
	scene->cylinders = malloc(sizeof(t_plane) * MAX_CYLINDERS);
	if (!scene->cylinders)
		finish(global, ERR_MEM);
	scene->num_cy = 0;
	return (MLX_SUCCESS);
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

int	main(int argc, char **argv)
{
	t_global	*global;

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
	global->vars.mlx_win = new_window(global);
	if (!global->vars.mlx_win)
		finish(global, ERR_WIN);
	// background(&global->img);
	render(global);
	save_bmp(&global->img, WIN_W - MARGIN, WIN_H - MARGIN, "ray_tracing.bmp");
	mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
		global->img.img, MARGIN / 2, MARGIN / 2);
	set_hooks(global);
	mlx_loop(global->vars.mlx_conn);
	return (0);
}
