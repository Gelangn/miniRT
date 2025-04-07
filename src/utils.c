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
	ft_printf("Exiting program\n");
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

void	save_bmp(t_img *img, int width, int height, const char *filename)
{
	// Información de diagnóstico
	ft_printf("Guardando imagen: width=%d, height=%d\n", width, height);
	ft_printf("img->bits_per_pixel=%d, img->line_length=%d\n",
		img->bits_per_pixel, img->line_length);

	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd < 0)
	{
		perror("Error al abrir el archivo");
		return ;
	}

	write_bmp_header(fd, width, height); // Escribir la cabecera BMP

	// Recorre los píxeles desde la parte inferior hacia arriba (formato BMP)
	for (int y = height - 1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			if (!is_valid_pixel(x, y, width, height))
				continue ;

			// Calcula la posición del píxel en el buffer de la imagen
			char *pixel = img->addr + (y * img->line_length + x
					* (img->bits_per_pixel / 8));

			// Extrae los componentes de color (asumiendo formato BGRA)
			unsigned char b = pixel[0];
			unsigned char g = pixel[1];
			unsigned char r = pixel[2];

			// Escribe los componentes de color en el archivo BMP
			if (write(fd, &b, 1) != 1 || write(fd, &g, 1) != 1 || write(fd, &r,
					1) != 1)
			{
				perror("Error al escribir datos de píxeles");
				close(fd);
				return ;
			}
		}

		// Escribe el relleno (padding) para alinear las filas a múltiplos de 4 bytes
		int padding = (4 - (width * 3) % 4) % 4;
		int i = -1;
		while (++i < padding)
		{
			if (write(fd, "\0", 1) != 1)
			{
				perror("Error al escribir relleno");
				close(fd);
				return ;
			}
		}
	}

	close(fd);
	ft_printf("Imagen guardada como %s\n", filename);
}