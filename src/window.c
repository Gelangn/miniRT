// incluir cabecera 42

#include "../inc/minirt.h"

int	close_window(t_global *global)
{
	printf("Closing window\n");
	// No liberar global aquí, solo marcar la ventana como cerrada
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
	return (0);
}

void	*new_window(t_global *global)
{
	void	*win;

	printf("Creating window\n");
	win = mlx_new_window(global->vars.mlx_conn, WIN_W, WIN_H, "miniRT");
	if (!win)
		return (NULL);
	// Liberar imagen anterior si existe
	if (global->img.img)
	{
		mlx_destroy_image(global->vars.mlx_conn, global->img.img);
		global->img.img = NULL;
		global->img.addr = NULL;
	}
	// Crear nueva imagen
	global->img.img = mlx_new_image(global->vars.mlx_conn, WIN_W, WIN_H);
	if (!global->img.img)
	{
		mlx_destroy_window(global->vars.mlx_conn, win);
		return (NULL);
	}
	global->img.addr = mlx_get_data_addr(global->img.img, &global->img.bpp,
			&global->img.bpl, &global->img.endian);
	if (!global->img.addr)
	{
		mlx_destroy_image(global->vars.mlx_conn, global->img.img);
		mlx_destroy_window(global->vars.mlx_conn, win);
		return (NULL);
	}
	printf("Window created\n");
	return (win);
}
