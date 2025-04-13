// incluir cabecera 42

#include "../inc/minirt.h"

void	pixel_put(t_img *img, int px_x, int px_y, int color)
{
	char	*dst;

	// Verificación completa de límites y validez
	if (!img || !img->addr || px_x < 0 || px_x >= WIN_W || px_y < 0
		|| px_y >= WIN_H)
		return ;
	// Calcular posición y escribir color
	dst = img->addr + (px_y * img->bpl + px_x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}
