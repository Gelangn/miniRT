/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bde-mada <bde-mada@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:16:39 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/18 18:39:51 by bde-mada         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../inc/prototype.h"
# include "../inc/struct.h"
# include "../lib/libft/ft_printf.h"
# include "../lib/libft/get_next_line.h"
# include "../lib/libft/libft.h"
# include "../lib/minilibx-linux/mlx.h"
# include <X11/keysym.h>
# include <errno.h>
# include <fcntl.h>
# include <linux/limits.h>
# include <math.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <unistd.h>

# define WIN_W 840 // Screen width 1024 840
# define WIN_H 630 // Screen height 768 630
# define MARGIN 50 // Margin for the screen

# define PI 3.14159265358979323846f
# define MAX_PLANES 5
# define MAX_SPHERES 100
# define MAX_CYLINDERS 15
# define VECTOR 0 // 0 for vector, 1 for point
# define POINT 1

# define DSCR 1.0f      // Distance from the camera to the screen
# define EPSILON 0.001f // Epsilon for floating point comparison

# define MLX_ERROR 1
# define MLX_SUCCESS 0
# define SUCCESS 0
# define FAILURE 1

# define ESC 53
# define ESC_KEY 65307

# define DEFAULT_COLOR MAGENTA
# define GROUND_COLOR GREY

# define WHITE 0xFFFFFF
# define RED 0xFF0000
# define GREEN 0x00FF00
# define BLUE 0x0000FF
# define YELLOW 0xFFFF00
# define CYAN 0x00FFFF
# define MAGENTA 0xFF00FF
# define BLACK 0x000000
# define GREY 0x454545
# define LIGHT_GREY 0xAAAAAA
# define DARK_GREY 0x111111
# define BROWN 0x8B4513
# define ORANGE 0xFFA500
# define PINK 0xFFC0CB
# define PURPLE 0x800080
# define MY_COLOR 0x89CE94

# define ARGS "Usage: ./miniRT <file.rt>\n"
# define ERR_ARGS \
	"\nIncorrect number of arguments, expected 1 Usage: \
 ./miniRT <file.rt>\n "
# define ERR_OPEN "\nError opening the file\n"
# define ERR_READ "\nError reading the file\n"
# define ERR_SPLIT "\nError splitting the line\n"
# define ERR_LINE "\nError: nº of elements is different for each line\n"
# define ERR_EMPTY "\nError: the file is empty or wrongly formatted\n"
# define ERR_MEM "\nCache error\n"
# define ERR_SCENE "\nScene Error\n"
# define ERR_MALLOC "\nMalloc error\n"
# define ERR_INVALID_EXT "\nError extension .rt\n"
# define ERR_AMBIENT "\nError ambient light\n"
# define ERR_CAMERA "\nError camera\n"
# define ERR_LIGHT "\nError light\n"
# define ERR_SPHERE "\nError sphere\n"
# define ERR_PLANE "\nError plane\n"
# define ERR_CYLINDER "\nError cylinder\n"
# define ERR_RESOLUTION "\nError resolution\n"
# define ERR_COLOR "\nError color\n"
# define ERR_FLOAT "\nError float\n"
# define ERR_VECTOR "\nError vector\n"
# define ERR_NORMAL "\nError normal\n"
# define ERR_ORIENTATION "\nError orientation\n"
# define ERR_FOV "\nError FOV\n"
# define ERR_POSITION "\nError position\n"
# define ERR_PARSE "\nError parsing\n"
# define ERR_WIN "\nError creating window\n"
# define ERR_IMG "\nError: Image not initialized correctly\n"
# define ERR_MLX "\nError initializing MLX\n"
# define ERR_SAVE "\nError saving BMP\n"

# define MAX_RAY_DEPTH 8
# define MIN_CONTRIBUTION 0.01f
# define AIR_REFRACTIVE_INDEX 1.0f

#endif
