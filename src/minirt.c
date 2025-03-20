// incluir cabecera 42

#include "../inc/minirt.h"
#include <linux/limits.h>

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
	global->img.img = NULL;
	global->img.addr = NULL;
	global->scene = (t_scene){0};
	ft_printf("Initializing MLX\n");
	global->vars.mlx_conn = mlx_init();
	ft_printf("Initialized MLX OK\n");
	return (MLX_SUCCESS);
}
static int initialize_scene(t_scene *scene)
{
	scene->fd = 0;
	scene->file_path = NULL;
	scene->lines = NULL;
	scene->scale = 0;
	scene->num_spheres = 0;
	scene->num_planes = 0;
	scene->num_cylinders = 0;
	// Inicializa otros campos según sea necesario
	return (MLX_SUCCESS);
}


// Function to check the file extension
void	check_file_extension(const char *filename)
{
	const char	*ext = ft_strrchr(filename, '.');

	if (ft_strcmp(ext, ".rt") != 0)
	{
		finish(ERR_INVALID_EXTENSION);
	}
	else if(!ext)
	{
		finish(ERR_ARGS);
	}
}

int	main(int argc, char **argv)
{
	t_global	*global;

	if (argc != 2)
	{
		//ft_printf(ERR_ARGS);
		fprintf(stderr, "Usage: %s <scene_file>\n", argv[0]);
		return (EXIT_FAILURE);
	}
	global = (t_global *)malloc(sizeof(t_global));
	if (!global)
	{
		perror("Error allocating memory for global");
		if (global)
			free(global);
		return EXIT_FAILURE;
	}
	check_file_extension(argv[1]);
	if ((initialize(global) != 0) || (initialize_scene(&global->scene) != 0))
		finish(ERR_MEM);
	global->scene.file_path = argv[1];
	printf("Opening file: %s\n", global->scene.file_path); 
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
