// incluir cabecera 42

#include "../inc/minirt.h"

int	close_window(t_global *global)
{
	printf("Closing window\n");
	global->vars.mlx_win = NULL;  // Solo marcar como cerrado, no destruir
	global->img.img = NULL;       // Solo marcar como cerrado, no destruir
	return (SUCCESS);
}

void	*new_window(t_global *global)
{
	void	*win;

	printf("Creating window\n");
	win = mlx_new_window(global->vars.mlx_conn, WIN_W, WIN_H, "miniRT");
	if (!win)
		finish(global, ERR_WIN);
	global->img.img = mlx_new_image(global->vars.mlx_conn, WIN_W - MARGIN, WIN_H
			- MARGIN);
	if (!global->img.img)
		finish(global, ERR_IMG);
	global->img.addr = mlx_get_data_addr(global->img.img, &global->img.bpp,
			&global->img.bpl, &global->img.endian);
	if (!global->img.addr)
	{
		mlx_destroy_image(global->vars.mlx_conn, global->img.img);
		mlx_destroy_window(global->vars.mlx_conn, win);
		finish(global, ERR_IMG);
	}
	printf("Window created\n");
	return (win);
}
