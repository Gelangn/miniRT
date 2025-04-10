#ifndef PROTOTYPE_H
# define PROTOTYPE_H

# include "../inc/minirt.h"
# include "../inc/struct.h"
# include <stddef.h>

/***** PROTOTYPE *****/
/* Main */
int			init(t_global *global);

/* Init */

/* Save BMP */

/* MLX y eventos  */
int			handle_no_event(void);
int			handle_keypress(int keysym, t_global *global);
int			handle_keyrelease(int keysym);
int			handle_mouse_move(int x, int y);
int			mouse_press_hook(int button, int x, int y);
int			mouse_release_hook(int button, int x, int y);
void		set_hooks(t_global *global);
int			handle_mouse_scroll(int button, int x, int y, t_global *global);

/* Window */
int			close_window(t_global *global);
void		*new_window(t_global *global);

/* scene */
void		read_scene(t_global *global);
void		free_scene(t_scene *scene);

/* Graphics-Render-Draw */
// void		render(t_global *global);
// void		points_3d_to_2d(t_scene *scene);
// void		isometric(t_point *point);
// void		scale_scene(t_scene *scene);
// void		draw_line(t_img *img, t_dot *dot, int color);
void		pixel_put(t_img *data, int x, int y, int color);
int			rgb_to_int(t_color color);

/* Utils */
char		*get_next_line(int fd);
char		*ft_strdup(const char *s);
char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strchr(const char *s, int c);
int			get_char_len(const char *chain, char chr);
void		finish(t_global *global, const char *message);
void		free_global(t_global *global);
void		dbl_free(char **ptr);
void		check_scene(t_global *global, t_scene *scene);
int			ft_atoi_base(const char *str, int base);
int			comp_floats(float a, float b);
void		save_bmp(t_img *img, int width, int height, const char *filename);
void		write_bmp_header(int fd, int width, int height);
int			open_bmp_file(const char *filename);
int			write_pixel(int fd, char *pixel);
int			write_padding(int fd, int width);
int			write_bmp_row(int fd, t_img *img, int y, int width);

/* Parsing */
void		parse_ambient(t_global *global, t_scene *scene, char *line);
void		parse_cam(t_global *global, t_scene *scene, char *line);
void		parse_light(t_global *global, t_scene *scene, char *line);
void		parse_sphere(t_global *global, t_scene *scene, char *line);
void		parse_plane(t_global *global, t_scene *scene, char *line);
void		parse_cylinder(t_global *global, t_scene *scene, char *line);

/* Raytracing */
t_intersec	col_sp(t_sphere *sphere, t_vector ray_origin, t_vector ray_dir);
void		render(t_global *global);
float		cal_discriminant(t_vector oc, t_vector ray_dir, float radius);
t_intersec	find_closest_intersec(t_global *global, t_vector ray_origin,
				t_vector ray_dir);
t_vector	get_ray_direction(t_camera cam, int pixel_x, int pixel_y);
t_vector	get_surface_normal(t_global *global, t_intersec intersec);

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

/* Innecesary */
t_vector	transpose(t_vector a);
t_vector	rotate(t_vector a, t_vector b, float angle);
t_vector	rotate_x(t_vector a, float angle);
t_vector	rotate_y(t_vector a, float angle);
t_vector	rotate_z(t_vector a, float angle);

#endif