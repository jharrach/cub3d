/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:31:47 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/01 16:32:34 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	partition(t_entity *arr, int i, int end)
{
	int			pivotindex;
	float		pivotvalue;
	t_entity	tmp;

	pivotindex = i;
	pivotvalue = arr[end].del_pos.y;
	while (i < end)
	{
		if (arr[i].del_pos.y > pivotvalue)
		{
			tmp = arr[i];
			arr[i] = arr[pivotindex];
			arr[pivotindex] = tmp;
			pivotindex++;
		}
		i++;
	}
	tmp = arr[pivotindex];
	arr[pivotindex] = arr[end];
	arr[end] = tmp;
	return (pivotindex);
}

static void	quick_sort_entities(t_entity *arr, int start, int end)
{
	int	i;

	if (start >= end)
		return ;
	i = partition(arr, start, end);
	quick_sort_entities(arr, start, i - 1);
	quick_sort_entities(arr, i + 1, end);
}

static void	update_entity_del_pos(t_data *data)
{
	int32_t	i;

	i = 0;
	while (i < data->num_entities)
	{
		data->entity[i].del_pos.x = data->pos.x - data->entity[i].pos.x;
		data->entity[i].del_pos.y = data->entity[i].pos.y - data->pos.y;
		data->entity[i].del_pos = \
			rotate_vec2f(data->entity[i].del_pos, data->dir);
		i++;
	}
}

static void	draw_entity(t_entity *entity, t_data *data)
{
	t_vec2i	loc;
	int32_t	j;
	float	len;

	loc.x = entity->del_pos.x * (data->dis / entity->del_pos.y);
	loc.x += data->win_wh;
	len = data->dis / entity->del_pos.y;
	loc.x -= len / 2.0;
	loc.y = ((int)data->win->height - len) / 2;
	j = 0;
	while (j < len)
	{
		if (loc.x >= 0 && loc.x < (int)data->win->width && \
			(len > data->ray_lenghts[loc.x] || \
				data->ray_lenghts[loc.x] == 0.0))
		{
			txt_to_img(data->win_entities, entity->img, \
				loc, j / len);
		}
		loc.x++;
		j++;
	}
}

void	ft_entities(t_data *data)
{
	int32_t	i;

	update_entity_del_pos(data);
	quick_sort_entities(data->entity, 0, data->num_entities - 1);
	ft_memset(data->win_entities->pixels, 0, \
		data->win->width * data->win->height * 4);
	i = 0;
	while (i < data->num_entities)
	{
		if (data->entity[i].enabled && data->entity[i].del_pos.y > 0.05)
			draw_entity(&(data->entity[i]), data);
		i++;
	}
}
