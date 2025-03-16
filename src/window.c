// incluir cabecera 42

#include "../inc/minirt.h"

int	close_window(void *mlx)
{
	t_global	*global;

	ft_printf("Closing window\n");
	mlx = NULL;
	if (mlx != NULL)
	{
		global = (t_global *)mlx;
		if (global->vars.mlx_conn != NULL && global->vars.mlx_win != NULL)
		{
			ft_printf("Destroying window\n");
			mlx_destroy_window(global->vars.mlx_conn, global->vars.mlx_win);
		}
	}
	return (MLX_SUCCESS);
}

void	*new_window(t_global *global)
{
	ft_printf("Creating window\n");
	global->vars.mlx_win = mlx_new_window(global->vars.mlx_conn, WIN_W, WIN_H, \
			"Welcome to miniRT - anavas-g");
	global->img.img = mlx_new_image(global->vars.mlx_conn, WIN_W - MARGIN, WIN_H
			- MARGIN);
	global->img.addr = mlx_get_data_addr(global->img.img,
			&global->img.bits_per_pixel, &global->img.line_length,
			&global->img.endian);
	if (!global->vars.mlx_win)
	{
		free(global->vars.mlx_win);
		return (NULL);
	}
	ft_printf("Window created\n");
	mlx_string_put(global->vars.mlx_conn, global->vars.mlx_win, \
		WIN_W * .89, WIN_H * .96, 0x00FF0000, "By anavas-g");
	return (global->vars.mlx_win);
}
