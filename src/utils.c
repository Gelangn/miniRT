// incluir cabecera 42

#include "../inc/minirt.h"

void	finish(t_global *global, const char *message)
{
	if (errno == 0)
		ft_putendl_fd((char *)message, 2);
	else
		perror(message);
	if (global)
	{
		if (global->vars.mlx_conn || global->vars.mlx_win)
			close_window(global);
		free_global(global);
	}
	printf("Exiting program\n");
	exit(1);
}

// Function to compare two floats using EPSILON
int	comp_floats(float a, float b)
{
	return (fabs(a - b) < EPSILON);
}

int	ft_atoi_base(const char *str, int base)
{
	int	neg;
	int	res;
	int	i;

	neg = 1;
	res = 0;
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-')
		neg = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && (ft_isdigit(str[i]) || (str[i] >= 'a' && str[i] <= 'f')
			|| (str[i] >= 'A' && str[i] <= 'F')))
	{
		if (ft_isdigit(str[i]))
			res = res * base + str[i] - '0';
		else if (str[i] >= 'a' && str[i] <= 'f')
			res = res * base + str[i] - 'a' + 10;
		else if (str[i] >= 'A' && str[i] <= 'F')
			res = res * base + str[i] - 'A' + 10;
		i++;
	}
	return (res * neg);
}

int	is_valid_pixel(int x, int y, int width, int height)
{
	return (x >= 0 && x < width && y >= 0 && y < height);
}

void	write_bmp_header(int fd, int width, int height)
{
	unsigned char	header[54] = {0};

	header[0] = 'B';
	header[1] = 'M';
	*(int *)&header[2] = 54 + (width * height * 3 + height * ((4 - (width * 3)
					% 4) % 4)); // Tamaño total del archivo con padding
	header[10] = 54;
	// Offset donde empiezan los datos de la imagen
	header[14] = 40;
	// Tamaño del encabezado DIB
	*(int *)&header[18] = width;
	*(int *)&header[22] = -height; // Altura negativa para que no se invierta
	header[26] = 1;                // Planos
	header[28] = 24;               // Bits por píxel (RGB - 24 bits)
	if (write(fd, header, 54) != 54)
	{
		perror("Error al escribir el encabezado BMP");
		close(fd);
		exit(EXIT_FAILURE);
	}
}

// Abre el archivo para guardar la imagen BMP
int	open_bmp_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd < 0)
	{
		perror("Error al abrir el archivo");
	}
	return (fd);
}

// Escribe un píxel en el archivo
int	write_pixel(int fd, char *pixel)
{
	unsigned char	b;
	unsigned char	g;
	unsigned char	r;

	b = pixel[0];
	g = pixel[1];
	r = pixel[2];
	if (write(fd, &b, 1) != 1 || write(fd, &g, 1) != 1 || write(fd, &r, 1) != 1)
	{
		perror("Error al escribir datos de píxeles");
		return (0);
	}
	return (EXIT_FAILURE);
}

// Escribe el padding al final de cada fila
int	write_padding(int fd, int width)
{
	int	padding;
	int	i;

	padding = (4 - (width * 3) % 4) % 4;
	i = -1;
	while (++i < padding)
	{
		if (write(fd, "\0", 1) != 1)
		{
			perror("Error al escribir relleno");
			return (0);
		}
	}
	return (EXIT_FAILURE);
}

// Escribe una fila de píxeles optimizada
int	write_bmp_row(int fd, t_img *img, int y, int width)
{
	int				x;
	char			*pixel;
	unsigned char	*row_buffer;
	int				padding;
	int				row_size;

	padding = (4 - (width * 3) % 4) % 4;
	row_size = width * 3 + padding;
	// Crear un buffer para toda la fila, incluyendo padding
	row_buffer = malloc(row_size);
	if (!row_buffer)
		return (0);
	// Llenar el buffer con datos de píxeles
	x = -1;
	while (++x < width)
	{
		pixel = img->addr + (y * img->bpl + x * (img->bpp / 8));
		row_buffer[x * 3] = pixel[0];     // B
		row_buffer[x * 3 + 1] = pixel[1]; // G
		row_buffer[x * 3 + 2] = pixel[2]; // R
	}
	// Añadir padding al final (ceros)
	memset(row_buffer + width * 3, 0, padding);
	// Escribir toda la fila de una vez
	if (write(fd, row_buffer, row_size) != row_size)
	{
		free(row_buffer);
		return (0);
	}
	free(row_buffer);
	return (1); // Éxito (en lugar de EXIT_FAILURE)
}

// Función principal que coordina el guardado de la imagen
void	save_bmp(t_img *img, int width, int height, const char *filename)
{
	int fd;
	int y;

	// Información de diagnóstico
	printf("Saving image: width=%d, height=%d\n", width, height);
	printf("img->bpp=%d, img->bpl=%d\n", img->bpp, img->bpl);

	fd = open_bmp_file(filename);
	if (fd < 0)
		return ;

	write_bmp_header(fd, width, height);

	// Escribir filas de arriba hacia abajo para coincidir con la altura negativa
	y = -1;
	while (++y < height)
	{
		if (!write_bmp_row(fd, img, y, width))
		{
			close(fd);
			return ;
		}
	}

	close(fd);
	printf("Image saved as %s\n", filename);
}