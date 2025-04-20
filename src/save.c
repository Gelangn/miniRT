/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 18:09:52 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 18:15:59 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

static void	fill_row_buffer(t_global *global, unsigned char *buf, int px_y,
		int width)
{
	int		px_x;
	char	*pixel;

	px_x = -1;
	while (++px_x < width)
	{
		pixel = global->img.addr + (px_y * global->img.bpl + px_x
				* (global->img.bpp / 8));
		buf[px_x * 3] = pixel[0];
		buf[px_x * 3 + 1] = pixel[1];
		buf[px_x * 3 + 2] = pixel[2];
	}
}

int	write_bmp_row(t_global *global, int fd, int px_y)
{
	int				width;
	int				padding;
	int				row_size;
	unsigned char	*row_buffer;

	width = WIN_W - MARGIN;
	padding = (4 - (width * 3) % 4) % 4;
	row_size = width * 3 + padding;
	row_buffer = malloc(row_size);
	if (!row_buffer)
		finish(global, ERR_MEM);
	fill_row_buffer(global, row_buffer, px_y, width);
	memset(row_buffer + width * 3, 0, padding);
	if (write(fd, row_buffer, row_size) != row_size)
	{
		free(row_buffer);
		finish(global, ERR_SAVE);
	}
	free(row_buffer);
	return (FAILURE);
}

void	write_bmp_header(t_global *global, int fd)
{
	unsigned char	header[54];
	int				width;
	int				height;

	width = WIN_W - MARGIN;
	height = WIN_H - MARGIN;
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
	write_bmp_header(global, fd);
	px_y = -1;
	while (++px_y < height)
	{
		if (!write_bmp_row(global, fd, px_y))
		{
			close(fd);
			return ;
		}
	}
	close(fd);
	printf("Image saved as %s\n", filename);
}
