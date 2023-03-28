/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:29:48 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/28 15:53:44 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <math.h>
# include <stdbool.h>
# include "../libft/include/libft.h"
# include "../mlx42/include/MLX42/MLX42.h"

# define WIDTH 2000
# define HEIGHT 1000
# define FOV 90.0f
# define PI 3.141592653589793

typedef struct	s_vec2f
{
	float	x;
	float	y;
}	t_vec2f;

typedef struct	s_vec2i
{
	int	x;
	int	y;
}	t_vec2i;

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
**/
typedef struct	s_data
{
	mlx_texture_t	*texture[4];
	t_vec2f		pos;
	t_vec2i		map_size;
	int			**map;
	mlx_t		*mlx;
	mlx_image_t	*win;
	float		dir;
	float		dir_delta;
	float		fov;
	float		dis;
	uint32_t	col_floor;
	uint32_t	col_ceiling;
	uint32_t	win_wh;
	mlx_image_t *prev_text;
}	t_data;

#endif
