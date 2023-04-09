/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 01:30:35 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/09 02:38:46 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_realloc_linebuffer(t_data *data)
{
	t_linef			*tmp;
	static int32_t	size = 0;

	if (data->mm.num_lines < size)
		return ;
	size += LINEBUFFERSIZE;
	if (!data->mm.line)
	{
		data->mm.line = ft_alloc(data, size, sizeof(*(data->mm.line)));
		return ;
	}
	tmp = data->mm.line;
	data->mm.line = ft_alloc(data, size, sizeof(*(data->mm.line)));
	ft_memcpy(data->mm.line, tmp, \
		sizeof(*(data->mm.line)) * (size - LINEBUFFERSIZE));
	ft_free(data, tmp);
}

void	ft_create_door(t_data *data, int32_t x, int32_t y)
{
	if (y > 0 && data->map[x][y - 1] != 1 && y + 1 < data->map_size.y && \
		data->map[x][y + 1] != 1)
	{
		ft_realloc_linebuffer(data);
		data->mm.line[data->mm.num_lines].a.x = x + 1;
		data->mm.line[data->mm.num_lines].a.y = y + 0.5;
		data->mm.line[data->mm.num_lines].b.x = x;
		data->mm.line[data->mm.num_lines].b.y = y + 0.5;
		data->mm.num_lines++;
	}
	if (x > 0 && data->map[x - 1][y] != 1 && x + 1 < data->map_size.x && \
		data->map[x + 1][y] != 1)
	{
		ft_realloc_linebuffer(data);
		data->mm.line[data->mm.num_lines].a.x = x + 0.5;
		data->mm.line[data->mm.num_lines].a.y = y + 1;
		data->mm.line[data->mm.num_lines].b.x = x + 0.5;
		data->mm.line[data->mm.num_lines].b.y = y;
		data->mm.num_lines++;
	}
}

static void	fill(mlx_image_t *img, uint32_t col)
{
	int32_t	i;

	i = img->width * img->height;
	while (i--)
		((uint32_t *)img->pixels)[i] = col;
}

void	draw_minimap(t_data *data)
{
	int32_t	i;
	t_vec2i	a;
	t_vec2i	b;
	t_vec2f	af;
	t_vec2f	bf;

	fill(data->mm.win, 0xAA000000);
	i = -1;
	while (++i < data->mm.num_lines)
	{
		af = data->mm.line[i].a;
		af.x -= data->pos.x;
		af.y -= data->pos.y;
		af = rotate_vec2f(af, -data->dir - PI);
		bf = data->mm.line[i].b;
		bf.x -= data->pos.x;
		bf.y -= data->pos.y;
		bf = rotate_vec2f(bf, -data->dir - PI);
		a.x = af.x * data->mm.scale + data->mm.win_h.x;
		a.y = af.y * data->mm.scale + data->mm.win_h.y;
		b.x = bf.x * data->mm.scale + data->mm.win_h.x;
		b.y = bf.y * data->mm.scale + data->mm.win_h.y;
		draw_line(data->mm.win, a, b, 0xFFFFFFFF);
	}
}
