/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 14:29:48 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/25 17:10:02 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include "../libft/include/libft.h"
# include "../mlx42/include/MLX42/MLX42.h"
# include "math.h"

# define WIDTH 1280
# define HEIGHT 960
# define FOV 66.0f
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

typedef struct data
{
	mlx_t *mlx;
	mlx_image_t *image;
	float player_direction;
	int (*map)[7][8];
	t_vec2f player_location;
	t_vec2f forward;
	double time;
	mlx_texture_t *north;
	mlx_texture_t *south;
	mlx_image_t *prev_text;
} t_data;

#endif
