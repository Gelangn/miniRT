// incluir cabecera 42

#include "../inc/minirt.h"

int	close_window(t_global *global)
{
	if (!global)
	{
		printf("Error: global is NULL\n");
		return (MLX_ERROR);
	}
	printf("Closing window\n");
	if (global->img.img)
	{
		printf("Destroying image\n");
		mlx_destroy_image(global->vars.mlx_conn, global->img.img);
		global->img.img = NULL;
	}
	if (global->vars.mlx_win)
	{
		printf("Destroying window\n");
		mlx_destroy_window(global->vars.mlx_conn, global->vars.mlx_win);
		global->vars.mlx_win = NULL;
	}
	if (global->vars.mlx_conn)
	{
		printf("Destroying display\n");
		mlx_destroy_display(global->vars.mlx_conn);
		free(global->vars.mlx_conn);
		global->vars.mlx_conn = NULL;
	}
	finish(global, NULL);
	printf("Window closed\n");
	exit(0);
	return (MLX_SUCCESS);
}

void	*new_window(t_global *global)
{
	printf("Creating window\n");
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
	global->img.addr = mlx_get_data_addr(global->img.img, &global->img.bpp,
			&global->img.bpl, &global->img.endian);
	printf("Window created\n");
	mlx_string_put(global->vars.mlx_conn, global->vars.mlx_win, WIN_W * .89,
			WIN_H * .96, MAGENTA, "By anavas-g");
	return (global->vars.mlx_win);
}
