#ifndef STRUCT_H
#define STRUCT_H

#include "../inc/minirt.h"
#include "../inc/prototype.h"

/**** STRUCTS ****/

// struct for bitscene
typedef struct s_img
{
	void *img; // image to be drawn on the window
	char *addr;
	int bits_per_pixel;
	int line_length;
	int endian;
} t_img;

// struct for mlx

typedef struct s_var
{
	void *mlx_conn; // connection to the window
	void *mlx_win;	// window to be drawn
} t_var;

typedef struct s_color
{
	int r;
	int g;
	int b;
} t_color;

typedef struct s_vector
{
	float x;
	float y;
	float z;
} t_vector;

typedef struct s_ambient
{
	float intensity;
	t_color color;
} t_ambient;

typedef struct s_camera
{
	int fov;
	t_vector position;
	t_vector orientation;
} t_camera;

typedef struct s_light
{
	float intensity;
	t_vector position;
} t_light;

typedef struct s_sphere
{
	float radius;
	t_vector center;
	t_color color;
} t_sphere;

typedef struct s_plane
{
	t_vector point;
	t_vector normal;
	t_color color;
} t_plane;

typedef struct s_cylinder
{
	t_vector base;
	t_vector orientation;
	float radius;
	float height;
	t_color color;
} t_cylinder;

typedef struct s_scene
{
	t_ambient ambient;
	t_camera camera;
	t_light light;

	int num_spheres;
	int num_planes;
	int num_cylinders;
	t_sphere *spheres;
	t_plane *planes;
	t_cylinder *cylinders;

	float scale;	 // scale of the scene
	int fd;			 // file descriptor
	char *file_path; // path of the file
	char **lines;
} t_scene;

// global struct to store all the structs
typedef struct s_global
{
	t_var vars;
	t_img img;
	t_scene scene;
} t_global;

#endif