#ifndef STRUCT_H
# define STRUCT_H

# include "../inc/minirt.h"
# include "../inc/prototype.h"

/**** STRUCTS ****/

// struct for bitscene
typedef struct s_img
{
	void *img; // image to be drawn on the window
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

// struct for points

typedef struct s_point
{
	int point_x;  // x coordinate of the point in the scene
	int point_y;  // y coordinate of the point in the scene
	int point_z;  // z coordinate of the point in the scene
	int color;    // color of the point in the scene
	float scrn_x; // coordinate x in the screen
	float scrn_y; // coordinate y in the screen
}			t_point;

typedef struct s_dot
{
	float	xi;
	float	xf;
	float	yi;
	float	yf;
}			t_dot;

// struct for scene

typedef struct s_scene
{
	t_point *points; // array of points from scene
	t_dot *dot;      // array of dots from scene
	char **lines;    // array of lines from scene
	int fd;          // file descriptor
	char *arg_path;  // path of the file
	int width;       // width of the scene
	int height;      // height of the scene
	int nr_elems;    // number of elements in the scene (width * height)
	float scale;     // scale of the scene
	int origin_x;    // origin x of the scene
	int origin_y;    // origin y of the scene
}			t_scene;

// global struct to store all the structs

typedef struct s_global
{
	t_var	vars;
	t_img	img;
	t_scene	scene;
}			t_global;

#endif