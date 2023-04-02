/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:37:14 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/01 17:00:09 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_door(t_data *data)
{
	if (data->door.moved >= 0.0)
	{
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
}

void	ft_check_door(t_data *data)
{
	t_vec2i	next_cell;
	t_recti	rect;

	next_cell.x = floorf(data->pos.x - sinf(data->dir));
	next_cell.y = floorf(data->pos.y + cosf(data->dir));
	if (next_cell.x >= 0 && next_cell.x < data->map_size.x && \
		next_cell.y >= 0 && next_cell.y < data->map_size.y)
	{
		if (data->map[next_cell.x][next_cell.y] == 3)
			data->door.opens = !data->door.opens;
		else if (data->door.moved >= 0.0)
			return ;
		else if (data->map[next_cell.x][next_cell.y] == 2)
		{
			data->map[next_cell.x][next_cell.y] = 3;
			data->door.cell = next_cell;
			data->door.moved = 0.0;
			data->door.opens = true;
		}
		else if (data->map[next_cell.x][next_cell.y] == -1)
		{
			data->map[next_cell.x][next_cell.y] = 3;
			init_recti_center_vec2f(&rect, data->pos, 0.25);
			if (recti_collide_map(&rect, data->map, data->map_size))
			{
				data->map[next_cell.x][next_cell.y] = -1;
				return ;
			}
			data->door.cell = next_cell;
			data->door.moved = 1.0;
			data->door.opens = false;
		}
	}
}
