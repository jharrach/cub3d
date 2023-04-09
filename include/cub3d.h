/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 01:27:40 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/09 02:47:27 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <math.h>
# include <stdbool.h>
# include <stdlib.h>
# include <fcntl.h>
# include "../libft/include/libft.h"
# include "../mlx42/include/MLX42/MLX42.h"

# define WIDTH 960
# define HEIGHT 540
# define FOV 90.0f
# define MOUSE_MUL 0.001
# define AIM_ZOOM 0.3
# define MM_SCALE 50.0
# define LINEBUFFERSIZE 10
# define PI 3.141592653589793
# define SHIFT_MULTIPLIER 2.0
# define CONTROL_MULTIPLIER 0.5
# define PLAYER_HALF_WIDTH 0.25
# define ENTITY_ANIMATION_MULTIPLIER 5.0
# define ENTITY_TEXTURE_CNT 8
# define ENTITY_BAR_COLOR 0xFF1CD7F9
# define TEXTURE_CNT 19

typedef enum e_input_types
{
	INVALID,
	NEWLINE,
	TEXTURE,
	COLOR
}	t_input_types;

typedef enum e_tex
{
	NO,
	SO,
	EA,
	WE,
	DO,
	E1,
	E2,
	E3,
	E4,
	E5,
	E6,
	E7,
	E8,
	BG,
	MM,
	B1,
	B2,
	B3,
	B4
}	t_tex;

/**
 * Input data for parsing
 * @param fd File descriptor to the input file
 * @param input The input data read from the file
 * @param i Pointer to next line next to processed
**/
typedef struct s_input
{
	char	**input;
	char	**i;
	int		fd;
}	t_input;

typedef struct s_vec2f
{
	float	x;
	float	y;
}	t_vec2f;

typedef struct s_vec2i
{
	int	x;
	int	y;
}	t_vec2i;

typedef struct s_rectf
{
	t_vec2f	tr;
	t_vec2f	tl;
	t_vec2f	br;
	t_vec2f	bl;
}	t_rectf;

typedef struct s_recti
{
	t_vec2i	tr;
	t_vec2i	tl;
	t_vec2i	br;
	t_vec2i	bl;
}	t_recti;

/**
 * @param unit length per cell
 * @param length horizontel and vertical lengths until next cell collision
 * @param cell_pos
 * @param cell_dir 1 when position increases, -1 when it decreases
 * @param angle
 * @param cos cosine of angle
 * @param sin sine of angle
 * @param len distence from start to collision
 * @param hoz is true when ray hits colliding cell horizontally
*/
typedef struct s_ray
{
	t_vec2f	unit;
	t_vec2f	length;
	t_vec2i	cell_pos;
	t_vec2i	cell_dir;
	float	angle;
	float	cos;
	float	sin;
	float	len;
	bool	hoz;
}	t_ray;

/**
 * @param cell position in map
 * @param moved distance door moved already
 * @param opens opens/closes
*/
typedef struct s_door
{
	t_vec2i	cell;
	float	moved;
	bool	opens;
}	t_door;

typedef struct s_linef
{
	t_vec2f	a;
	t_vec2f	b;
}	t_linef;

typedef struct s_entity
{
	t_rectf			rect;
	t_vec2f			pos;
	t_vec2f			del_pos;
	bool			enabled;
	float			half_width;
}	t_entity;

typedef struct s_mm
{
	t_vec2f		win_h;
	mlx_image_t	*win;
	mlx_image_t	*win_tex;
	t_linef		*line;
	int32_t		num_lines;
	float		scale;
}	t_mm;

/**
 * @param texture Wall textures
 * @param pos Player position
 * @param map_size
 * @param map
 * @param mlx
 * @param win Main image
 * @param dir Player direction in radians
 * @param fov Filed of view in radians
 * @param dis Distance from pov to window
 * @param col_floor
 * @param col_ceiling
 * @param win_wh Half width of main image
 * @param door
**/
typedef struct s_data
{
	mlx_texture_t	*texture[TEXTURE_CNT];
	t_input			in;
	t_mm			mm;
	t_door			door;
	t_vec2f			pos;
	t_vec2i			map_size;
	int				**map;
	mlx_t			*mlx;
	mlx_image_t		*win;
	mlx_image_t		*win_entities;
	t_list			*head;
	t_entity		*entity;
	float			dir;
	float			dir_delta;
	float			fov;
	float			dis;
	float			*ray_lenghts;
	float			*ray_angle;
	float			animation;
	uint32_t		col_floor;
	uint32_t		col_ceiling;
	uint32_t		win_wh;
	int32_t			num_entities;
	int32_t			collected;
	bool			menu;
}	t_data;

/** @file button.c */
void			ft_menu(t_data *data);

/** @file destroy.c */
/**
 * @param int The input data
 * @param ext Whether it should exit
 * @param error An optional error message
**/
void			destroy_data(t_data *data, bool ext, char *error);

/** @file door.c */
void			ft_door(t_data *data);
void			ft_check_door(t_data *data);

/** @file drawing.c */
void			draw_line(mlx_image_t *img, t_vec2i a, t_vec2i b, uint32_t col);
void			draw_rectangle(mlx_image_t *img, t_vec2i start, t_vec2i size, \
					int col);
void			txt_to_img(mlx_image_t *dst, mlx_texture_t *src, t_vec2i loc, \
					float x_hit);

/** @file entity_collision.c */
void			collide_entity(t_data *data);

/** @file entity.c */
void			ft_entities(t_data *data);

/** @file fps.c */
void			ft_fps(t_data *data);

/** @file hooks.c */
void			ft_loop_hook(void *param);
void			ft_scroll_hook(double xdelta, double ydelta, void *param);
void			ft_resize_hook(int32_t width, int32_t height, void *param);
void			ft_key_hook(mlx_key_data_t keydata, void *param);
void			ft_mouse_hook(mouse_key_t button, action_t action, \
					modifier_key_t mods, void *param);

/** @file init.c */
/**
 * @param fn The path of the input file
**/
void			init_data(t_data *data, char *fn);

/** @file key_input.c */
void			get_key_input(t_data *data);

/** @file memory.c */
void			*ft_alloc(t_data *data, size_t count, size_t size);
void			*ft_alloc_add(t_data *data, void *content);
void			ft_free(t_data *data, void *content);

/** @file minimap.c */
void			ft_create_minimap(t_data *data);

/** @file minimap2.c */
void			ft_realloc_linebuffer(t_data *data);
void			ft_create_door(t_data *data, int32_t x, int32_t y);
void			draw_minimap(t_data *data);

/** @file mouse_input.c */
void			get_mouse_input(t_data *data);

/** @file parsing_map.c */
void			load_map(t_data *d, t_input *in);

/** @file parsing_utils.c */
void			ft_free2d(char **arr);
const char		*get_ident(int i);
t_input_types	get_input_type(char *line);
int				cnt_spaces(char *str);
int				check_ints(char **str);

/** @file parsing.c */
void			load_data(t_data *data, t_input *in);

/** @file ray_collision.c */
void			ft_cast_ray(t_ray *ray, t_data *data, int32_t i);

/** @file ray.c */
void			ft_rays(t_data *data);
void			update_ray_angles(t_data *data);

/** @file rectangle.c */
bool			recti_collide_map(t_recti *rect, int **map, t_vec2i map_size);
void			init_recti_center_vec2f(t_recti *rect, t_vec2f center, \
					float halfwidth);
void			init_rectf_center_vec2f(t_rectf *rect, t_vec2f center, \
					float halfwidth);

/*** @file textures.c */
float			texture_width(mlx_texture_t *tex);
void			texture_processing(t_data *data);
void			scale_texture_to_img(mlx_texture_t *texture, \
					mlx_image_t *image);

/** @file vector.c */
t_vec2f			vec2f_mul_f(t_vec2f v, float m);
t_vec2f			rotate_vec2f(t_vec2f v1, float angle);

#endif //CUB3D_H
