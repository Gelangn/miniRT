// incluir cabecera 42

#include "../inc/minirt.h"

void	finish(t_global *global, const char *message)
{
	if (errno == 0)
		ft_putendl_fd((char *)message, 2);
	else
		perror(message);
	if (global)
		free_global(global); // Que free_global haga toda la limpieza
	printf("Exiting program\n");
	exit(SUCCESS);
}

// Function to compare two floats using EPSILON
int	comp_floats(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

// Devuelve true si a es menor que b-EPSILON
int	is_less_than(float a, float b)
{
	return (a < b - EPSILON);
}

// Devuelve true si a es mayor que b+EPSILON
int	is_greater_than(float a, float b)
{
	return (a > b + EPSILON);
}

int	is_valid_pixel(int px_x, int px_y, int width, int height)
{
	return (px_x >= 0 && px_x < width && px_y >= 0 && px_y < height);
}

// Antes: void write_bmp_header(int fd, t_global *global)
void	write_bmp_header(t_global *global, int fd)
{
	unsigned char	header[54];
	int				width;
	int				height;

	width = WIN_W - MARGIN;
	height = WIN_H - MARGIN;
	// Inicializar todo el header a cero
	memset(header, 0, 54);
	header[0] = 'B';
	header[1] = 'M';
	*(int *)&header[2] = 54 + (width * height * 3 + height * ((4 - (width * 3)
					% 4) % 4));
	header[10] = 54;
	header[14] = 40;
	*(int *)&header[18] = width;
	*(int *)&header[22] = -height;
	header[26] = 1;
	header[28] = 24;
	if (write(fd, header, 54) != 54)
	{
		close(fd);
		finish(global, ERR_SAVE);
	}
}

// Antes: int write_bmp_row(int fd, int px_y, t_global *global)
int	write_bmp_row(t_global *global, int fd, int px_y)
{
	int				width;
	int				padding;
	int				row_size;
	unsigned char	*row_buffer;
	int				px_x;
	char			*pixel;

	width = WIN_W - MARGIN;
	padding = (4 - (width * 3) % 4) % 4;
	row_size = width * 3 + padding;
	row_buffer = malloc(row_size);
	if (!row_buffer)
		finish(global, ERR_MEM);
	px_x = -1;
	while (++px_x < width)
	{
		pixel = global->img.addr + (px_y * global->img.bpl + px_x
				* (global->img.bpp / 8));
		row_buffer[px_x * 3] = pixel[0];     // B
		row_buffer[px_x * 3 + 1] = pixel[1]; // G
		row_buffer[px_x * 3 + 2] = pixel[2]; // R
	}
	memset(row_buffer + width * 3, 0, padding);
	if (write(fd, row_buffer, row_size) != row_size)
	{
		free(row_buffer);
		finish(global, ERR_SAVE);
	}
	free(row_buffer);
	return (FAILURE);
}

void	save_bmp(t_global *global, const char *filename)
{
	int	fd;
	int	px_y;
	int	width;
	int	height;

	width = WIN_W - MARGIN;
	height = WIN_H - MARGIN;
	printf("Saving image: width=%d, height=%d\n", width, height);
	printf("img->bpp=%d, img->bpl=%d\n", global->img.bpp, global->img.bpl);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		finish(global, ERR_OPEN);
	write_bmp_header(global, fd); // Cambiado el orden
	px_y = -1;
	while (++px_y < height)
	{
		if (!write_bmp_row(global, fd, px_y)) // Cambiado el orden
		{
			close(fd);
			return ;
		}
	}
	close(fd);
	printf("Image saved as %s\n", filename);
}
