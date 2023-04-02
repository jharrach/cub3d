/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2023/04/02 23:34:13 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <math.h>
# include <stdbool.h>
# include <fcntl.h>
# include "../libft/include/libft.h"
# include "../mlx42/include/MLX42/MLX42.h"

# define WIDTH 1440
# define HEIGHT 810
# define FOV 90.0f
# define AIM_ZOOM 0.3
# define LINEBUFFERSIZE 10
# define PI 3.141592653589793
# define SHIFT_MULTIPLIER 2.0
# define CONTROL_MULTIPLIER 0.5
# define PLAYER_HALF_WIDTH 0.25
# define TEXTURE_CNT 6

typedef enum e_input_types
{
	INVALID,
	NEWLINE,
	TEXTURE,
	COLOR
}	t_input_types;

/**
 * Input data for parsing
 * @param fd File descriptor to the input file
 * @param input The input data read from the file
 * @param i Pointer to next line next to processed
**/
typedef struct s_input
{
	int		fd;
	char	**input;
	char	**i;
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
	t_vec2f			pos;
	t_vec2f			del_pos;
	mlx_texture_t	*img;
	bool			enabled;
	float			half_width;
}	t_entity;

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
	t_vec2f			pos;
	t_vec2i			map_size;
	int				**map;
	mlx_t			*mlx;
	mlx_image_t		*win;
	mlx_image_t		*win_entities;
	float			dir;
	float			dir_delta;
	float			fov;
	float			dis;
	float			*ray_lenghts;
	float			*ray_angle;
	uint32_t		col_floor;
	uint32_t		col_ceiling;
	uint32_t		win_wh;
	t_door			door;
	t_entity		*entity;
	int32_t			num_entities;
	t_input			in;
	t_linef			*mini_map;
	int32_t			mm_size;
	mlx_image_t		*mm_win;
	t_vec2i			mm_win_h;
	mlx_image_t		*mm_txt;
}	t_data;

//init
/**
 * @param fn The path of the input file
**/
void			init_data(t_data *data, char *fn);

//parsing
void			load_data(t_data *data, t_input *in);

//parsing_map
void			load_map(t_data *d, t_input *in);

//parsing_utils
void			ft_free2d(char **arr);
const char		*get_ident(int i);
t_input_types	get_input_type(char *line);
int				cnt_spaces(char *str);
int				check_ints(char **str);

//destroy
/**
 * @param int The input data
 * @param ext Whether it should exit
 * @param error An optional error message
**/
void			destroy_data(t_data *data, bool ext, char *error);

void			ft_rays(t_data *data);
void			draw_rectangle(mlx_image_t *img, t_vec2i start , t_vec2i size, int col);
int32_t			factor_pixel(int c, float f);
void			txt_to_img(mlx_image_t *dst, mlx_texture_t *src, t_vec2i loc, float x_hit);

void	ft_entities(t_data *data);

void	ft_fps(t_data *data);

void	ft_door(t_data *data);
void	ft_check_door(t_data *data);

void	get_key_input(t_data *data);
void	get_mouse_input(t_data *data);

void	ft_loop_hook(void *param);
void	ft_scroll_hook(double xdelta, double ydelta, void *param);
void	ft_resize_hook(int32_t width, int32_t height, void *param);
void	ft_key_hook(mlx_key_data_t keydata, void *param);
void	ft_mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param);

void	ft_create_minimap(t_data *data);
void	draw_minimap(t_data *data);

void	ft_rays(t_data *data);
void	ft_cast_ray(t_ray *ray, t_data *data, int32_t i);
void	update_ray_angles(t_data *data);
int32_t	factor_pixel(int c, float f);
void	txt_to_img(mlx_image_t *dst, mlx_texture_t *src, t_vec2i loc, float x_hit);
void	init_recti_center_vec2f(t_recti *rect, t_vec2f center, float halfwidth);
void	init_rectf_center_vec2f(t_rectf *rect, t_vec2f center, float halfwidth);
bool	recti_collide_map(t_recti *rect, int **map, t_vec2i map_size);

t_vec2f	vec2f_mul_f(t_vec2f v, float m);
t_vec2f	vec2f_add(t_vec2f v1, t_vec2f v2);
t_vec2f	vec2f_sub(t_vec2f v1, t_vec2f v2);
t_vec2f	rotate_vec2f(t_vec2f v1, float angle);

#endif
