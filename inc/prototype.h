#ifndef PROTOTYPE_H
# define PROTOTYPE_H

# include "../inc/minirt.h"
# include "../inc/struct.h"
# include <stddef.h>

/***** PROTOTYPES *****/
/* Main - init */
int			init(t_global *global);
int			init_scene(t_global *global, t_scene *scene);
t_intersec	init_intersec(void);
void		init_lateral_intersec_vars(t_cylinder *cylinder,
				t_vector ray_origin, t_vector ray_dir, t_cyl_lat *vars);

/* MLX y eventos  */
void		set_hooks(t_global *global);
int			handle_no_event(void);
int			handle_keypress(int keysym, t_global *global);
int			handle_mouse_move(int x, int y, t_global *global);
int			handle_mouse_scroll(int button, int x, int y, t_global *global);
int			mouse_press_hook(int button, int x, int y, t_global *global);
int			mouse_release_hook(int button, int x, int y, t_global *global);

/* Camera controls*/
void		move_camera(t_global *global, t_vector direction, float distance);
void		rotate_camera(t_global *global, t_vector axis, float angle);

/* Window */
int			close_window(t_global *global);
void		*new_window(t_global *global);

/* scene */
void		read_scene(t_global *global);
void		check_scene(t_global *global, t_scene *scene);
void		free_scene(t_scene *scene);

/* Graphics-Render-Draw */
void		pixel_put(t_img *data, int x, int y, int color);
int			rgb_to_int(t_color color);

/* Utils */
char		*get_next_line(int fd);
void		finish(t_global *global, const char *message);
void		free_global(t_global *global);
void		dbl_free(char **ptr);
int			comp_floats(float a, float b);
int			is_less_than(float a, float b);
int			is_greater_than(float a, float b);

/* Save BMP */
void		save_bmp(t_global *global, const char *filename);
void		write_bmp_header(int fd, t_global *global);
int			write_bmp_row(int fd, int y, t_global *global);

/* Parsing */
void		parse_ambient(t_global *global, t_scene *scene, char *line);
void		parse_cam(t_global *global, t_scene *scene, char *line);
void		parse_light(t_global *global, t_scene *scene, char *line);
void		parse_sphere(t_global *global, t_scene *scene, char *line);
void		parse_plane(t_global *global, t_scene *scene, char *line);
void		parse_cylinder(t_global *global, t_scene *scene, char *line);

/* Raytracing */
void		render(t_global *global);
float		cal_discriminant(t_vector oc, t_vector ray_dir, float radius);
t_intersec	find_closest_intersec(t_global *global);
void		trace_all_rays(t_global *global, t_intersec *intersecs);
t_intersec	cal_ray(t_global *global, int pixel_x, int pixel_y);
t_vector	get_ray_direction(t_camera cam, int pixel_x, int pixel_y);
t_vector	get_surface_normal(t_global *global, t_intersec intersec);
void		check_sp_intersecs(t_global *global, t_intersec *closest_intersec);
void		check_pl_intersecs(t_global *global, t_intersec *closest_intersec);
void		check_cy_intersecs(t_global *global, t_intersec *closest_intersec);

/* Colisions */
t_intersec	col_sp(t_sphere *sphere, t_global *global);
t_intersec	col_pl(t_plane *plane, t_global *global);
t_intersec	col_cy(t_cylinder *cylinder, t_global *global);
float		cal_lateral_discriminant(t_cylinder *cylinder, t_cyl_lat vars);
t_intersec	cal_lateral_intersec(t_cylinder *cylinder, t_global *global);
t_intersec	cal_cap_intersec(t_cylinder *cylinder, t_global *global,
				int cap_sign);
t_intersec	check_lateral_hits(t_cylinder *cylinder, t_global *global,
				t_cyl_lat vars);
t_intersec	process_lateral_hit(t_cylinder *cylinder, t_global *global,
				t_cyl_lat vars, float t);
t_vector	get_cap_center(t_cylinder *cylinder, t_vector axis, int cap_sign);
t_vector	get_cap_normal(t_vector axis, int cap_sign);
t_vector	get_cy_normal(t_global *global, t_intersec intersec);
t_vector	get_pl_normal(t_global *global, t_intersec intersec);
t_vector	get_sp_normal(t_global *global, t_intersec intersec);
t_vector	get_surface_normal(t_global *global, t_intersec intersec);
void		get_intersec_points(float a, float b, float discriminant,
				t_cyl_lat *vars);
/* Colors*/
t_color		cal_lighting(t_global *global, t_intersec intersec,
				t_vector ray_dir);

/* Matrix */
t_vector	add(t_vector a, t_vector b);
t_vector	subtract(t_vector a, t_vector b);
float		dot(t_vector a, t_vector b);
t_vector	multiply(t_vector a, float scalar);
t_vector	divide(t_vector a, float scalar);
float		magnitude(t_vector a);
t_vector	normalize(t_vector a);
t_vector	cross(t_vector a, t_vector b);
t_vector	reflect(t_vector a, t_vector b);

#endif