// incluir cabecera 42

#include "../inc/minirt.h"

static void	background(t_img *data)
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
}

static int	initialize(t_global *global)
{
	//global->scene.points = NULL;
	global->scene.file_path = NULL;
	global->scene.fd = 0;
	global->img.img = NULL;
	global->img.addr = NULL;
	global->scene = (t_scene){0};
	ft_printf("Initializing mlx\n");
	global->vars.mlx_conn = mlx_init();
	ft_printf("MLX initialized OK\n");
	
	return (MLX_SUCCESS);
}
void initialize_scene(t_scene *scene)
{
	scene->num_spheres = 0;
	scene->num_planes = 0;
	scene->num_cylinders = 0;
	// Inicializa otros campos según sea necesario
}


// Function to check the file extension
void	check_file_extension(const char *filename)
{
	const char	*ext = ft_strrchr(filename, '.');

	if (!ext || ft_strcmp(ext, ".rt") != 0)
	{
		finish(ERR_INVALID_EXTENSION);
	}
}

int	main(int argc, char **argv)
{
	t_global	*global;
	//t_scene		scene;

	if (argc != 2)
	{
		ft_printf(ERR_ARGS);
		return (0);
	}
	global = (t_global *)malloc(sizeof(t_global));
	global->scene.file_path = argv[1];
	check_file_extension(global->scene.file_path);
	initialize(global);
	initialize_scene(&global->scene);
	read_scene(&global->scene, global->scene.file_path);
	global->vars.mlx_win = new_window(global);
	
	background(&global->img);
	//render(global);
	mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
		global->img.img, MARGIN / 2, MARGIN / 2);
	set_hooks(global);
	mlx_loop(global->vars.mlx_conn);
	return (0);
}
