/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anavas-g <anavas-g@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 17:16:59 by anavas-g          #+#    #+#             */
/*   Updated: 2025/06/21 14:42:37 by anavas-g         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

# include "../inc/minirt.h"
# include "../inc/struct.h"

/***** PROTOTYPES *****/
/* Initialization */
void		init(t_global *global);
void		init_scene(t_global *global);
t_intersec	init_isec(void);

/* Window and MLX */
void		*new_window(t_global *global);
void		set_hooks(t_global *global);

/* Event handling */
int			handle_keypress(int keysym, t_global *global);
int			handle_mouse_move(int x, int y, t_global *global);
int			handle_mouse_scroll(int button, int x, int y, t_global *global);
int			mouse_press_hook(int button, int x, int y, t_global *global);
int			mouse_release_hook(int button, int x, int y, t_global *global);
void		handle_movement_keys(t_global *global, int keysym,
				int *needs_render);
void		handle_rotation_keys(t_global *global, int keysym,
				int *needs_render);
void		handle_zoom_keys(t_global *global, int keysym);
void		handle_screenshot(t_global *global);
void		update_display(t_global *global);

/* Camera controls */
void		move_camera(t_global *global, t_vector direction, float distance);
void		rotate_camera(t_global *global, t_vector axis, float angle);
void		precal_camera_axis(t_global *global);
void		precal_rays(t_global *global);

/* Camera rotation functions */
void		apply_standard_rotation(t_global *global, int keysym,
				float rotate_speed);
void		apply_roll_rotation(t_global *global, int keysym,
				float rotate_speed);

/* Scene handling */
void		count_objects(t_global *global);
void		allocate_objects(t_global *global);
void		read_scene(t_global *global);
void		check_scene(t_global *global, t_scene *scene);
void		free_scene(t_scene *scene);
void		parse_ambient(t_global *global, char *line);
void		parse_cam(t_global *global, char *line);
void		parse_light(t_global *global, char *line);
void		parse_sphere(t_global *global, char *line);
void		parse_plane(t_global *global, char *line);
void		parse_cylinder(t_global *global, char *line);

/* Utilities */
char		*get_next_line(int fd);
void		finish(t_global *global, const char *message);
void		free_global(t_global *global);
void		dbl_free(char **ptr);
int			comp_floats(float a, float b);
int			is_less_than(float a, float b);
int			is_greater_than(float a, float b);
void		replace_tabs_with_spaces(char *str);
float		parse_float_token(t_global *global, char **tokens);
int			parse_int_token(t_global *global, char **tokens);
void		parse_color(t_global *global, char *token, t_color *color);
void		parse_vector(t_global *global, char *token, t_vector *vector);
int			is_valid_float(char *str);

/* BMP functions */
void		write_bmp_header(t_global *global, int fd);
int			write_bmp_row(t_global *global, int fd, int px_y);
void		save_bmp(t_global *global, const char *filename);

/* Graphics and rendering */
void		pixel_put(t_img *img, int x, int y, int color);
void		render(t_global *global);
void		render_all_pixels(t_global *global);
void		render_single_pixel(t_global *global, int index);
void		trace_all_rays(t_global *global);
int			get_pixel_color(t_global *global, int index);

/* Ray tracing core */
void		cal_ray_for_pixel(t_global *global, int px_x, int px_y, int idx);
t_intersec	find_closest_isec(t_global *global);
t_vector	get_ray_direction(t_global *global, int pixel_x, int pixel_y);
t_vector	get_surface_normal(t_global *global, t_intersec isec);
void		check_obj_isecs(t_global *global, t_intersec *closest_isec,
				int obj_type);

/* Collision detection */
t_intersec	col_sp(t_global *global, int sp_id);
t_intersec	col_pl(t_global *global, int pl_id);
t_intersec	col_cy(t_global *global, int cy_id);

/* Cylinder intersection helpers */
t_intersec	cal_lateral_isec(t_global *global, int cy_id);
t_intersec	cal_cap_isec(t_global *global, int cy_id, int cap_sign);
void		init_lateral_isec_vars(t_global *global, int cy_id);
t_intersec	check_lateral_hits(t_global *global, int cy_id);
t_intersec	process_lateral_hit(t_global *global, int cy_id, float t);

/* Color operations */
int			rgb_to_int(t_color color);
t_color		color_scale(t_color color, float factor);
t_color		color_add(t_color c1, t_color c2);
void		clamp_color(t_color *color);

/* Lighting and shading */
t_color		get_object_color(t_global *global);
int			is_valid_isec(t_global *global);
void		prepare_lighting_data(t_global *global);
int			cal_shadow(t_global *global);
t_color		cal_ambient(t_global *global);
t_color		cal_diffuse(t_global *global);
t_color		cal_specular(t_global *global);
t_color		apply_lighting(t_global *global, int in_shadow);
t_color		cal_lighting(t_global *global);

/* Shadow ray utilities */
t_ray_state	save_ray_state(t_global *global);
void		restore_ray_state(t_global *global, t_ray_state state);

/* Vector/Matrix operations */
t_vector	add(t_vector a, t_vector b);
t_vector	subtract(t_vector a, t_vector b);
float		dot(t_vector a, t_vector b);
t_vector	multiply(t_vector a, float scalar);
t_vector	divide(t_vector a, float scalar);
float		mag(t_vector a);
t_vector	norm(t_vector a);
t_vector	cross(t_vector a, t_vector b);
t_vector	reflect(t_vector a, t_vector b);

/* Material properties */
float		get_object_transp(t_global *global, t_intersec isec);
float		get_object_reflct(t_global *global, t_intersec isec);
float		get_object_refractive_index(t_global *global, t_intersec isec);

/* Reflection and refraction */
t_vector	reflect_ray(t_vector incident, t_vector normal);
t_vector	refract_ray(t_vector incident, t_vector normal, float n1, float n2);
float		fresnel_reflectance(t_vector incident, t_vector normal, float n1,
				float n2);

/* Advanced lighting */
t_color		cal_lighting_advanced(t_global *global);
t_color		trace_ray_iterative(t_global *global, t_vector origin,
				t_vector direction, int max_depth);
t_color		cal_reflection(t_global *global, float reflct);
t_color		cal_transp(t_global *global, float transp);
t_color		mix_colors_by_transp(t_color basic, t_color trans, float transp);
void		backup_ray_state(t_global *global, t_ray_backup *backup);
void		restore_ray_state(t_global *global, t_ray_backup *backup);
t_vector	setup_reflection_ray(t_global *global);
t_vector	setup_transmission_ray(t_global *global);
t_color		apply_reflection_to_color(t_color final, t_color reflect, float reflct);

/* Signal handling */
void		init_signal_handlers(t_global *global);
void		handle_signal(int signum);

/* Object inside/outside testing */
int			is_inside_object(t_global *global, t_intersec isec,
				t_vector origin);
int			is_inside_sphere(t_sphere *sphere, t_vector point);

/* Refraction and reflection functions */
float		schlick(float cos_angle, float n1, float n2);

/* Prototipos para funciones de test de interior */
int			is_inside_sphere(t_sphere *sphere, t_vector point);
int			is_inside_cylinder(t_cylinder *cyl, t_vector point);
int			is_inside_object(t_global *global, t_intersec isec,
				t_vector origin);

#endif