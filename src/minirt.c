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
	ft_printf("Initializing mlx\n");
	global->vars.mlx_conn = mlx_init();
	ft_printf("MLX initialized OK\n");
	global->vars.mlx_win = new_window(&global->vars);
	global->map.points = NULL;
	global->map.arg_path = NULL;
	global->map.fd = 0;
	global->img.img = NULL;
	global->img.addr = NULL;
	global->map = (t_map){0};
	global->img.img = mlx_new_image(global->vars.mlx_conn, WIN_W - MARGIN, WIN_H
			- MARGIN);
	global->img.addr = mlx_get_data_addr(global->img.img,
			&global->img.bits_per_pixel, &global->img.line_length,
			&global->img.endian);
	background(&global->img);
	return (MLX_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_global	*global;

	if (argc != 2)
	{
		ft_printf(ERR_ARGS);
		return (0);
	}
	else
	{
		global = (t_global *)malloc(sizeof(t_global));
		initialize(global);
		read_map(&global->map, argv[1]);
		render(global);
		mlx_put_image_to_window(global->vars.mlx_conn, global->vars.mlx_win,
			global->img.img, MARGIN / 2, MARGIN / 2);
		set_hooks(global);
		mlx_loop(global->vars.mlx_conn);
	}
	return (0);
}
