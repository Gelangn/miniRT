#ifndef STRUCT_H
# define STRUCT_H

# include "../inc/minirt.h"
# include "../inc/prototype.h"

/**** STRUCTS ****/

// struct for bitscene
typedef struct s_img
{
	void *img;  // image pointer
	char *addr; // address of the image
	int bpp;    // bits per pixel
	int bpl;    // bytes per line
	int endian; // endian
}				t_img;
typedef struct s_var
{
	void *mlx_conn; // connection to the window
	void *mlx_win;  // window to be drawn
}				t_var;

// struct for points
typedef struct s_point
{
	float point_x; // x coordinate of the point in object space
	float point_y; // y coordinate of the point in object space
	float point_z; // z coordinate of the point in object space
	int color;     // color of the point in the map
	double scrn_x; // coordinate x in the screen
	double scrn_y; // coordinate y in the screen
}				t_point;

// struct for mlx

typedef struct s_color
{
	int r; // red component of the color
	int g; // green component of the color
	int b; // blue component of the color
}				t_color;

typedef struct s_vector
{
	float x; // x coordinate of the vector
	float y; // y coordinate of the vector
	float z; // z coordinate of the vector
}				t_vector;

typedef struct s_ambient
{
	float intensity; // intensity of the ambient light
	t_color color;   // color of the ambient light
	int			init;
}				t_ambient;

typedef struct s_camera
{
	int fov;              // field of view (grados o radianes)
	double dist_scrn;     // distance from the camera to the screen
	t_vector pos;         // position of the camera
	t_vector orientation; // orientation of the camera (vector)
	int			init;
}				t_camera;

typedef struct s_light
{
	float intensity; // intensity of the light
	t_vector pos;    // position of the light
	int			init;
}				t_light;

typedef struct s_intersec
{
	float dist;     // distance from the camera to the intersection point
	t_vector point; // intersection point
	int obj_index;  // Índice del objeto intersectado
	int obj_type;   // Tipo de objeto: 0-esfera, 1-plano, 2-cilindro
}				t_intersec;

typedef struct s_sphere
{
	float		radius;
	t_vector	center;
	t_color		color;
}				t_sphere;

typedef struct s_plane
{
	t_vector	point;
	t_vector	normal;
	t_color		color;
}				t_plane;

typedef struct s_cylinder
{
	t_vector base;        // Centro de la base inferior
	t_vector orientation; // Dirección normalizada del eje
	float radius;         // Radio del cilindro
	float height;         // Altura del cilindro
	t_color		color;
}				t_cylinder;

typedef struct s_cyl_lat
{
	t_vector axis;      // Eje normalizado del cilindro
	t_vector oc;        // Vector desde origen del rayo a la base del cilindro
	float dir_dot_axis; // Producto escalar de dirección-eje
	float oc_dot_axis;  // Producto escalar de oc-eje
	t_vector dir_perp;  // Componente perpendicular de dirección
	t_vector oc_perp;   // Componente perpendicular de oc
	float t1;           // Primera intersección potencial
	float t2;           // Segunda intersección potencial
}				t_cyl_lat;

typedef struct s_scene
{
	t_ambient	ambient;
	t_camera	cam;
	t_light		light;

	int			num_sp;
	int			num_pl;
	int			num_cy;
	t_sphere	*spheres;
	t_plane		*planes;
	t_cylinder	*cylinders;

	float scale; // scale of the scene

	int fd;          // file descriptor
	char *file_path; // path of the file
	char		**lines;
}				t_scene;

// global struct to store all the structs
typedef struct s_global
{
	t_var		vars;
	t_img		img;
	t_scene		scene;
	t_point		*points;
	t_intersec	intersec;
	int			mouse_pressed;
	int			last_mouse_x;
	int			last_mouse_y;

	// Variables para el rayo actual
	t_vector	current_ray_origin;
	t_vector	current_ray_dir;
}				t_global;

#endif