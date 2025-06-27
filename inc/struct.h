/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   struct.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:17:33 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/27 10:41:38 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCT_H
# define STRUCT_H

# include "../inc/minirt.h"
# include "../inc/prototype.h"

/**** STRUCTS ****/

// struct for bitscene
typedef struct s_img
{
	void			*img;
	char			*addr;
	int				bpp;
	int				bpl;
	int				endian;
}					t_img;
typedef struct s_var
{
	void			*mlx_conn;
	void			*mlx_win;
}					t_var;

// struct for points in object space and screen
typedef struct s_point
{
	float			point_x;
	float			point_y;
	float			point_z;
	int				color;
	double			scrn_x;
	double			scrn_y;
}					t_point;

// struct for mlx
typedef struct s_color
{
	int				r;
	int				g;
	int				b;
}					t_color;

typedef struct s_vector
{
	float			x;
	float			y;
	float			z;
}					t_vector;

/**
 * Material properties for rendered objects
 */
typedef struct s_material
{
	t_color			color;
	float			trans;
	float			refl;
	float			refr_idx;
}					t_material;
typedef struct s_ambient
{
	float			intens;
	t_color			color;
	int				init;
}					t_ambient;

typedef struct s_camera
{
	int				fov;
	double			dist_scrn;
	t_vector pos;        // Camera position in the world
	t_vector dir;        // Direction vector (where cam is looking)
	t_vector right_axis; // Camera right axis
	t_vector up_axis;    // Camera up axis
	t_vector fw_axis;    // Camera forward axis
	float roll_angle;    // New: Accumulated roll angle
	int				init;
}					t_camera;

typedef struct s_light
{
	float			intens;
	t_vector		pos;
	int				init;
}					t_light;

typedef struct s_intersec
{
	float			dist;
	t_vector		point;
	int				obj_index;
	int				obj_type;
}					t_intersec;

typedef struct s_sphere
{
	t_vector		center;
	float			radius;
	t_material		material;
}					t_sphere;

typedef struct s_plane
{
	t_vector		point;
	t_vector		normal;
	t_material		material;
}					t_plane;

typedef struct s_cylinder
{
	t_vector		base;
	t_vector		axis;
	float			radius;
	float			height;
	t_material		material;
}					t_cylinder;

typedef struct s_cyl_lat
{
	t_vector		axis;
	t_vector		oc;
	float			dir_dot_axis;
	float			oc_dot_axis;
	t_vector		dir_perp;
	t_vector		oc_perp;
	float			t1;
	float			t2;

	// Temporary variables for process_lateral_hit
	t_vector		hit_point;
	float			hit_h;
	t_vector		ctr_h;
	t_vector		normal;

	// Temporary variables for cal_cap_intersec
	t_vector		cap_center;
	float			dist_from_center;

	// Variables for discriminant calculation
	float			a;
	float			b;
	float			c;
	float			discr;
}					t_cyl_lat;

typedef struct s_scene
{
	t_ambient		ambient;
	t_camera		cam;
	t_light			light;

	int				num_sp;
	int				num_pl;
	int				num_cy;
	t_sphere		*spheres;
	t_plane			*planes;
	t_cylinder		*cyls;

	float			scale;

	int				fd;
	char			*file_path;
	char			**lines;
}					t_scene;

typedef struct s_ray_state
{
	t_vector		origin;
	t_vector		dir;
	t_intersec		isec;
}					t_ray_state;

typedef struct s_ray_result
{
	t_vector		origin;
	t_vector		direction;
	float contribution; // How much this ray contributes to final color
	int				depth;
	int is_inside; // For refraction cals (inside object or not)
}					t_ray_result;

typedef struct s_active_ray
{
	t_vector		origin;
	t_vector		dir;
	t_intersec		hit;
	t_color			obj_color;
	t_vector		normal;
}					t_active_ray;

typedef struct s_light_calc
{
	t_vector		dir;
	float			distance;
	t_color			obj_color;
	t_vector		normal;
}					t_light_calc;

// global struct to store all the structs
typedef struct s_global
{
	t_var			vars;
	t_img			img;
	t_scene			scene;
	t_point			*points;
	t_intersec		*isecs;
	int				mouse_pressed;
	int				last_mouse_x;
	int				last_mouse_y;
	int				total_pixels;

	// Variables for the current ray
	t_active_ray	c_ray;

	// Variables for lighting calculations
	t_light_calc	c_light;

	// Variables for cylinder calculations
	t_cyl_lat		current_cyl_vars;
}					t_global;

#endif