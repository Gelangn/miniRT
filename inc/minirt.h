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
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define WIN_W 600 // Screen width
# define WIN_H 400 // Screen height
# define MARGIN 50  // Margin for the screen

# define PI 3.14159265359f
# define MAX_PLANES 5
# define MAX_SPHERES 100
# define MAX_CYLINDERS 15
# define VECTOR 0 // 0 for vector, 1 for point
# define POINT 1

# define SCALAR 1
# define DSCR 1.0f      // Distance from the camera to the screen
# define EPSILON 0.0001 // Epsilon for floating point comparison

# define MLX_ERROR 1
# define MLX_SUCCESS 0

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
# define LIGHT_GREY 0xD3D3D3
# define DARK_GREY 0xA9A9A9
# define BROWN 0x8B4513
# define ORANGE 0xFFA500
# define PINK 0xFFC0CB
# define PURPLE 0x800080
# define MY_COLOR 0x89CE94

# define ARGS "Usage: ./miniRT <file.rt>\n"
# define ERR_ARGS "\nIncorrect number of arguments, expected 1 " ARGS
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

#endif
