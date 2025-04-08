// incluir cabecera 42

#include "../inc/minirt.h"

void	pixel_put(t_img *data, int x, int y, int color)
{
	char	*dst;
	
	dst = data->addr + ((y * data->bpl) + x * (data->bpp / 8));
	*(unsigned int *)dst = color;
}
