/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:31:47 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/12 01:14:49 by jharrach         ###   ########.fr       */
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

static void	txt_to_img2(mlx_image_t *dst, mlx_texture_t *src, \
	t_vec2i loc, float x_hit)
{
	int			index;
	t_vec2f		src_loc;
	int const	draw_height = dst->height - loc.y;
	float const	y_step = (float)src->width / (draw_height - loc.y);

	src_loc.y = src->height * x_hit;
	src_loc.x = 0;
	if (loc.y < 0)
	{
		src_loc.x = y_step * -loc.y;
		loc.y = 0;
	}
	while (loc.y < draw_height && loc.y < (int)dst->height)
	{
		index = (int)src_loc.y * src->width + src_loc.x;
		if (((int *)src->pixels)[index])
			((int *)dst->pixels)[loc.y * dst->width + loc.x] \
			= ((int *)src->pixels)[index];
		src_loc.x += y_step;
		loc.y++;
	}
}

static void	draw_entity(t_data *data, int32_t i)
{
	t_vec2i		loc;
	float const	len = data->dis / data->entity[i].del_pos.y;
	t_vec2f		mapped;

	loc.x = data->entity[i].del_pos.x * len + data->win_wh + len / 2.0;
	loc.y = ((int)data->win->height - len) / 2;
	mapped = (t_vec2f){.x = 0.0, .y = 1.0 / len};
	while (mapped.x < 1.0)
	{
		if (loc.x >= 0 && loc.x < (int)data->win->width && \
			(len > data->ray_lenghts[loc.x]) && data->entity[i].enabled)
		{
			txt_to_img2(data->win_entities, \
				data->texture[E1 + (int)data->animation], loc, mapped.x);
		}
		loc.x--;
		mapped.x += mapped.y;
	}
}

void	ft_entities(t_data *data)
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
	quick_sort_entities(data->entity, 0, data->num_entities - 1);
	ft_memset(data->win_entities->pixels, 0, \
		data->win->width * data->win->height * 4);
	i = 0;
	while (i < data->num_entities)
	{
		if (data->entity[i].del_pos.y > 0.05)
			draw_entity(data, i);
		i++;
	}
	data->animation += data->mlx->delta_time * ENTITY_ANIMATION_MULTIPLIER;
	if (data->animation >= ENTITY_TEXTURE_CNT)
		data->animation = 0.0;
	collide_entity(data);
}
