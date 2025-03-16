#ifndef STRUCT_H
# define STRUCT_H

# include "../inc/minirt.h"
# include "../inc/prototype.h"
# include "../inc/parsing.h"

/**** STRUCTS ****/

// struct for bitscene
typedef struct s_img
{
	void 	*img; // image to be drawn on the window
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

// struct for mlx

typedef struct s_var
{
	void *mlx_conn; // connection to the window
	void *mlx_win;  // window to be drawn
}			t_var;


// global struct to store all the structs

typedef struct s_global
{
	t_var	vars;
	t_img	img;
	t_scene	scene;
}			t_global;

#endif