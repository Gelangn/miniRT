// incluir cabecera 42

#include "../inc/minirt.h"

int	rgb_to_int(t_color color)
{
	return ((color.r << 16) | (color.g << 8) | color.b);
}