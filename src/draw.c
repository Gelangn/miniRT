// incluir cabecera 42

#include "../inc/minirt.h"

void	pixel_put(t_img *img, int x, int y, int color)
{
	char	*dst;

	// Verificación completa de límites y validez
	if (!img || !img->addr || x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return ;
	// Calcular posición y escribir color
	dst = img->addr + (y * img->bpl + x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}
