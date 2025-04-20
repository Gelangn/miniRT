/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:13:34 by anavas-g          #+#    #+#             */
/*   Updated: 2025/04/20 21:05:34 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minirt.h"

void	pixel_put(t_img *img, int px_x, int px_y, int color)
{
	char	*dst;

	if (!img || !img->addr || px_x < 0 || px_x >= WIN_W || px_y < 0
		|| px_y >= WIN_H)
		return ;
	dst = img->addr + (px_y * img->bpl + px_x * (img->bpp / 8));
	*(unsigned int *)dst = color;
}
