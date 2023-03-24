/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:29:48 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/24 18:14:00 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <math.h>
# include "../libft/include/libft.h"
# include "../mlx42/include/MLX42/MLX42.h"

# define WIDTH 1600
# define HEIGHT 900
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
 * @param map
 * @param mlx
 * @param win Main image
 * @param delta_time
 * @param dir Player direction in radians
 * @param fov Filed of view in radians
 * @param dis Distance from pov to window
 * @param col_floor
 * @param col_ceiling
 * @param win_wh Half width of main image
**/

typedef struct	s_data
{
	mlx_image_t	texture[4];
	t_vec2f		pos;
	int			**map;
	mlx_t		*mlx;
	mlx_image_t	*win;
	double		delta_time;
	float		dir;
	float		fov;
	float		dis;
	uint32_t	col_floor;
	uint32_t	col_ceiling;
	uint32_t	win_wh;
}	t_data;

#endif
