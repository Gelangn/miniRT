/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:16:39 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/27 10:38:11 by anavas-g         ###   ########.fr       */
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
# define MAX_PLANES 10
# define MAX_SPHERES 100
# define MAX_CYLINDERS 150
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
# define BACKGROUND_COLOR BLACK

# define BLACK 0x000000
# define WHITE 0xFFFFFF
# define RED 0xFF0000
# define GREEN 0x00FF00
# define BLUE 0x0000FF
# define DARK_GREY 0x202020
# define GREY 0x808080
# define YELLOW 0xFFFF00
# define CYAN 0x00FFFF
# define MAGENTA 0xFF00FF
# define LIGHT_GREY 0xAAAAAA
# define BROWN 0x8B4513
# define ORANGE 0xFFA500
# define PINK 0xFFC0CB
# define PURPLE 0x800080
# define MY_COLOR 0x89CE94

# define ARGS "Usage: ./miniRT <file.rt>\n"
# define ERR_ARGS \
	"Incorrect number of arguments, expected 1 Usage: \
 ./miniRT <file.rt>"
# define ERR_OPEN "Opening the file"
# define ERR_READ "Reading the file"
# define ERR_SPLIT "Splitting the line"
# define ERR_LINE "# of elements is different for each line"
# define ERR_EMPTY "The file is empty or wrongly formatted"
# define ERR_MEM "Cache error"
# define ERR_SCENE "Scene Error"
# define ERR_MALLOC "Malloc error"
# define ERR_INVALID_EXT "Extension .rt"
# define ERR_AMBIENT "Ambient light"
# define ERR_CAMERA "Camera"
# define ERR_LIGHT "Light"
# define ERR_SPHERE "Sphere"
# define ERR_PLANE "Plane"
# define ERR_CYLINDER "Cylinder"
# define ERR_RESOLUTION "Resolution"
# define ERR_COLOR "Color"
# define ERR_FLOAT "Float"
# define ERR_VECTOR "Vector"
# define ERR_NORMAL "Normal"
# define ERR_ORIENTATION "Orientation"
# define ERR_FOV "FOV"
# define ERR_POSITION "Position"
# define ERR_PARSE "Parsing"
# define ERR_WIN "Creating window"
# define ERR_IMG "Image not initialized correctly"
# define ERR_MLX "Initializing MLX"
# define ERR_SAVE "Saving BMP"

# define MAX_RAY_DEPTH 4        // Maximum depth for ray tracing
# define MIN_CONTRIBUTION 0.01f // Minimum contribution reflection/refraction
# define AIR_REFR_IDX 1.0f      // Refractive index of air

#endif
