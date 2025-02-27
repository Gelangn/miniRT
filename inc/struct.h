#ifndef STRUCT_H
# define STRUCT_H

/**** STRUCTS ****/

// struct for bitmap

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
	int point_x;  // x coordinate of the point in the map
	int point_y;  // y coordinate of the point in the map
	int point_z;  // z coordinate of the point in the map
	int color;    // color of the point in the map
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

// struct for map

typedef struct s_map
{
	t_point *points; // array of points from map
	t_dot *dot;      // array of dots from map
	char **lines;    // array of lines from map
	int fd;          // file descriptor
	char *arg_path;  // path of the file
	int width;       // width of the map
	int height;      // height of the map
	int nr_elems;    // number of elements in the map (width * height)
	float scale;     // scale of the map
	int origin_x;    // origin x of the map
	int origin_y;    // origin y of the map
}			t_map;

// global struct to store all the structs

typedef struct s_global
{
	t_var	vars;
	t_img	img;
	t_map	map;
}			t_global;

#endif