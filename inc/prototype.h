#ifndef PROTOTYPE_H
# define PROTOTYPE_H

# include "../inc/minirt.h"
# include "../inc/struct.h"
# include <stddef.h>

/***** PROTOTYPE *****/
/* MLX y eventos  */
int		handle_no_event(void);
int		handle_keypress(int keysym, t_global *global);
int		handle_keyrelease(int keysym);
int		handle_mouse_move(int x, int y);
int		mouse_press_hook(int button, int x, int y);
int		mouse_release_hook(int button, int x, int y);
void	set_hooks(t_global *global);

/* Window */
int		close_window(void *mlx);
void	*new_window(t_global *global);

/* scene */
void	read_scene(t_global *global);
void	free_scene(t_scene *scene);

/* Graphics-Render-Draw */
// void	render(t_global *global);
void	points_3d_to_2d(t_scene *scene);
// void	isometric(t_point *point);
// void	scale_scene(t_scene *scene);
// void	draw_line(t_img *img, t_dot *dot, int color);
void	pixel_put(t_img *data, int x, int y, int color);

/* Utils */
char	*get_next_line(int fd);
char	*ft_strdup(const char *s);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strchr(const char *s, int c);
int		get_char_len(const char *chain, char chr);
void	finish(t_global *global, const char *message);
void	free_global(t_global *global);
void	dbl_free(char **ptr);
void	check_scene(t_global *global, t_scene *scene);
//void	splited(t_scene *scene, int j, int i, int index);
int		ft_atoi_base(const char *str, int base);
int		comp_floats(float a, float b);

	/* Parsing */
void	parse_ambient(t_global *global, t_scene *scene, char *line);
void	parse_camera(t_global *global, t_scene *scene, char *line);
void	parse_light(t_global *global, t_scene *scene, char *line);
void	parse_sphere(t_global *global, t_scene *scene, char *line);
void	parse_plane(t_global *global, t_scene *scene, char *line);
void	parse_square(t_scene *scene, char *line);
void	parse_cylinder(t_global *global, t_scene *scene, char *line);
float	roundingf(float value, int decimals);

#endif