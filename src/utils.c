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

int	save_bmp(t_img *img, int width, int height, const char *filename)
{
	int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fd == -1)
	{
		perror("Error al abrir el archivo");
		return (-1);
	}

	int padding = (4 - (width * 3) % 4) % 4;
	int filesize = 14 + 40 + (width * 3 + padding) * height;

	unsigned char bmp_header[14] = {
		'B',
		'M',
		filesize,
		filesize >> 8,
		filesize >> 16,
		filesize >> 24,
		0,
		0,
		0,
		0,
		54,
		0,
		0,
		0,
	};

	unsigned char dib_header[40] = {
		40, 0, 0, 0, width, width >> 8, width >> 16, width >> 24, height,
			height >> 8, height >> 16, height >> 24, 1, 0, 24, 0, 0, 0, 0, 0, 0,
			0, 0, 0, 0, 0, 0, 0,
	};

	if (write(fd, bmp_header, 14) != 14)
	{
		perror("Error al escribir la cabecera BMP");
		close(fd);
		return (-1);
	}
	if (write(fd, dib_header, 40) != 40)
	{
		perror("Error al escribir la cabecera DIB");
		close(fd);
		return (-1);
	}

	for (int y = height - 1; y >= 0; y--)
	{
		for (int x = 0; x < width; x++)
		{
			int color = img->addr[y * width + x];
			unsigned char b = color & 0xFF;
			unsigned char g = (color >> 8) & 0xFF;
			unsigned char r = (color >> 16) & 0xFF;
			if (write(fd, &b, 1) != 1 || write(fd, &g, 1) != 1 || write(fd, &r,
					1) != 1)
			{
				perror("Error al escribir datos de píxeles");
				close(fd);
				return (-1);
			}
		}
		for (int i = 0; i < padding; i++)
		{
			if (write(fd, "\0", 1) != 1)
			{
				perror("Error al escribir relleno");
				close(fd);
				return (-1);
			}
		}
	}

	close(fd);
	return (0);
}