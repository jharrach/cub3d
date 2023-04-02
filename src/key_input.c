/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_input.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:44:51 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/01 16:45:33 by jharrach         ###   ########.fr       */
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

static bool	collide_entity(t_data *data)
{
	t_rectf	r1;
	t_rectf	r2;
	int32_t	i;

	init_rectf_center_vec2f(&r1, data->pos, PLAYER_HALF_WIDTH);
	i = -1;
	while (i++ < data->num_entities)
	{
		if (data->entity[i].del_pos.x > 2.0 || data->entity[i].del_pos.y > 2.0 || \
			data->entity[i].del_pos.x < -2.0 || data->entity[i].del_pos.y < -2.0 || \
			!data->entity[i].enabled)
			continue;
		init_rectf_center_vec2f(&r2, data->entity[i].pos, data->entity[i].half_width);
		if (data->entity[i].half_width > PLAYER_HALF_WIDTH)
		{
			if (r1.tr.x >= r2.tl.x && r1.tr.x <= r2.tr.x && \
				r1.tr.y >= r2.bl.y && r1.tr.y <= r2.tl.y)
				return (true);
			if (r1.tl.x >= r2.tl.x && r1.tl.x <= r2.tr.x && \
				r1.tl.y >= r2.bl.y && r1.tl.y <= r2.tl.y)
				return (true);
			if (r1.br.x >= r2.tl.x && r1.br.x <= r2.tr.x && \
				r1.br.y >= r2.bl.y && r1.br.y <= r2.tl.y)
				return (true);
			if (r1.bl.x >= r2.tl.x && r1.bl.x <= r2.tr.x && \
				r1.bl.y >= r2.bl.y && r1.bl.y <= r2.tl.y)
				return (true);
		}
		else
		{
			if (r2.tr.x >= r1.tl.x && r2.tr.x <= r1.tr.x && \
				r2.tr.y >= r1.bl.y && r2.tr.y <= r1.tl.y)
				return (true);
			if (r2.tl.x >= r1.tl.x && r2.tl.x <= r1.tr.x && \
				r2.tl.y >= r1.bl.y && r2.tl.y <= r1.tl.y)
				return (true);
			if (r2.br.x >= r1.tl.x && r2.br.x <= r1.tr.x && \
				r2.br.y >= r1.bl.y && r2.br.y <= r1.tl.y)
				return (true);
			if (r2.bl.x >= r1.tl.x && r2.bl.x <= r1.tr.x && \
				r2.bl.y >= r1.bl.y && r2.bl.y <= r1.tl.y)
				return (true);
		}
	}
	return (false);
}

void	get_key_input(t_data *data)
{
	t_vec2f	delta;
	t_recti	rect;

	delta = rotate_vec2f(player_delta(data->mlx), data->dir);
	data->pos.x += delta.x;
	init_recti_center_vec2f(&rect, data->pos, PLAYER_HALF_WIDTH);
	if (recti_collide_map(&rect, data->map, data->map_size) || collide_entity(data))
		data->pos.x -= delta.x;
	data->pos.y += delta.y;
	init_recti_center_vec2f(&rect, data->pos, PLAYER_HALF_WIDTH);
	if (recti_collide_map(&rect, data->map, data->map_size) || collide_entity(data))
		data->pos.y -= delta.y;
}
