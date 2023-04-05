/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:37:14 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/05 20:07:51 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_door(t_data *data)
{
	if (data->door.moved < 0.0)
		return ;
	if (data->door.opens)
	{
		data->door.moved += data->mlx->delta_time;
		if (data->door.moved > 1.0)
		{
			data->door.moved = -1.0;
			data->map[data->door.cell.x][data->door.cell.y] = -1;
		}
	}
	else
	{
		data->door.moved -= data->mlx->delta_time;
		if (data->door.moved < 0.0)
		{
			data->door.moved = -1.0;
			data->map[data->door.cell.x][data->door.cell.y] = 2;
		}
	}
}

void	ft_check_door(t_data *data)
{
	t_vec2i const	cell = {floorf(data->pos.x - sinf(data->dir)), \
		floorf(data->pos.y + cosf(data->dir))};
	t_recti			rect;

	if (cell.x < 0 || cell.x >= data->map_size.x || \
			cell.y < 0 || cell.y >= data->map_size.y)
		return ;
	if (data->map[cell.x][cell.y] == 3)
		data->door.opens = !data->door.opens;
	else if (data->door.moved < 0.0 && data->map[cell.x][cell.y] == 2)
	{
		data->map[cell.x][cell.y] = 3;
		data->door = (t_door){.cell = cell, .moved = 0.0, .opens = true};
	}
	else if (data->door.moved < 0.0 && data->map[cell.x][cell.y] == -1)
	{
		data->map[cell.x][cell.y] = 3;
		init_recti_center_vec2f(&rect, data->pos, PLAYER_HALF_WIDTH);
		if (recti_collide_map(&rect, data->map, data->map_size))
		{
			data->map[cell.x][cell.y] = -1;
			return ;
		}
		data->door = (t_door){.cell = cell, .moved = 1.0, .opens = false};
	}
}
