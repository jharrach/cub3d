/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:44:51 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/11 14:41:12 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static t_vec2f	player_delta(mlx_t *mlx)
{
	t_vec2f	delta;

	delta = (t_vec2f){.x = 0.0, .y = 0.0};
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_W))
		delta.y += mlx->delta_time;
	if (mlx_is_key_down(mlx, MLX_KEY_S))
		delta.y -= mlx->delta_time;
	if (mlx_is_key_down(mlx, MLX_KEY_A))
		delta.x += mlx->delta_time;
	if (mlx_is_key_down(mlx, MLX_KEY_D))
		delta.x -= mlx->delta_time;
	if (delta.x && delta.y)
		delta = vec2f_mul_f(delta, 0.70710678118f);
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT_SHIFT))
		delta = vec2f_mul_f(delta, SHIFT_MULTIPLIER);
	else if (mlx_is_key_down(mlx, MLX_KEY_LEFT_CONTROL))
		delta = vec2f_mul_f(delta, CONTROL_MULTIPLIER);
	return (delta);
}

void	get_key_input(t_data *data)
{
	t_vec2f	delta;
	t_recti	rect;

	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		data->dir_delta += data->mlx->delta_time;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		data->dir_delta -= data->mlx->delta_time;
	delta = rotate_vec2f(player_delta(data->mlx), data->dir);
	data->pos.x += delta.x;
	init_recti_center_vec2f(&rect, data->pos, PLAYER_HALF_WIDTH);
	if (recti_collide_map(&rect, data->map, data->map_size))
		data->pos.x -= delta.x;
	data->pos.y += delta.y;
	init_recti_center_vec2f(&rect, data->pos, PLAYER_HALF_WIDTH);
	if (recti_collide_map(&rect, data->map, data->map_size))
		data->pos.y -= delta.y;
}
