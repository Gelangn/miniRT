// incluir cabecera 42

#include "../inc/minirt.h"

int	close_window(t_global *global)
{
	ft_printf("Closing window\n");
	if (global->img.img != NULL && global->vars.mlx_conn != NULL)
	{
		ft_printf("Destroying image\n");
		mlx_destroy_image(global->vars.mlx_conn, global->img.img);
		global->img.img = NULL;
		// Asegurarse de que no se use después de ser destruido
	}
	if (global->vars.mlx_win != NULL && global->vars.mlx_conn != NULL)
	{
		ft_printf("Destroying window\n");
		mlx_destroy_window(global->vars.mlx_conn, global->vars.mlx_win);
		global->vars.mlx_win = NULL;
		// Asegurarse de que no se use después de ser destruido
	}
	free_global(global);
	ft_printf("Window closed\n");
	exit(0);
	return (MLX_SUCCESS);
}
void	*new_window(t_global *global)
{
	ft_printf("Creating window\n");
	global->vars.mlx_win = mlx_new_window(global->vars.mlx_conn, WIN_W, WIN_H,
			"Welcome to miniRT - anavas-g");
	if (!global->vars.mlx_win)
	{
		free(global->vars.mlx_win);
		return (NULL);
	}
	global->img.img = mlx_new_image(global->vars.mlx_conn, WIN_W - MARGIN, WIN_H
			- MARGIN);
	if (!global->img.img)
	{
		mlx_destroy_window(global->vars.mlx_conn, global->vars.mlx_win);
		global->vars.mlx_win = NULL;
		return (NULL);
	}
	global->img.addr = mlx_get_data_addr(global->img.img,
			&global->img.bits_per_pixel, &global->img.line_length,
			&global->img.endian);
	ft_printf("Window created\n");
	mlx_string_put(global->vars.mlx_conn, global->vars.mlx_win, WIN_W * .89,
		WIN_H * .96, 0x00FF0000, "By anavas-g");
	return (global->vars.mlx_win);
}
