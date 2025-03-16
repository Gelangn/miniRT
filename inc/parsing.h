#ifndef PARSING_H
# define PARSING_H

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
	t_vector position;
	t_vector orientation;
	int fov;
} t_camera;

typedef struct s_light
{
	t_vector position;
	float intensity;
} t_light;

typedef struct s_sphere
{
	t_vector center;
	float radius;
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
	t_sphere spheres[100]; // Adjust size as needed
	int num_spheres;
	t_plane planes[100]; // Adjust size as needed
	int num_planes;
	t_cylinder cylinders[100]; // Adjust size as needed
	int num_cylinders;
	float scale; // scale of the scene
	int fd; // file descriptor
	char *file_path; // path of the file
} t_scene;

void read_scene(t_scene *scene, const char *file_path);

#endif
// struct for points

/* typedef struct s_point
{
	int point_x;  // x coordinate of the point in the scene
	int point_y;  // y coordinate of the point in the scene
	int point_z;  // z coordinate of the point in the scene
	int color;    // color of the point in the scene
	float scrn_x; // coordinate x in the screen
	float scrn_y; // coordinate y in the screen
}			t_point;
 */
/* typedef struct s_dot
{
	float	xi;
	float	xf;
	float	yi;
	float	yf;
}			t_dot; */

// struct for scene

/* typedef struct s_scene
{
	// t_point *points; // array of points from scene
	//t_dot *dot;      // array of dots from scene
	char **lines;    // array of lines from scene

	int width;       // width of the scene
	int height;      // height of the scene
	int nr_elems;    // number of elements in the scene (width * height)
	
	int origin_x;    // origin x of the scene
	int origin_y;    // origin y of the scene
}			t_scene; */