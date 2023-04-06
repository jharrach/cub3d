/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jan-arvid <jan-arvid@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 21:16:53 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/06 14:17:52 by jan-arvid        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	ft_realloc_linebuffer(t_data *data)
{
	t_linef			*tmp;
	static int32_t	size = 0;

	if (data->mm_size < size)
		return ;
	size += LINEBUFFERSIZE;
	if (!data->mini_map)
	{
		data->mini_map = malloc(sizeof(*(data->mini_map)) * size);
		if (!data->mini_map)
			destroy_data(data, true, "malloc()");
		return ;
	}
	tmp = data->mini_map;
	data->mini_map = malloc(sizeof(*(data->mini_map)) * size);
	if (!data->mini_map)
	{
		free(tmp);
		destroy_data(data, true, "malloc()");
	}
	ft_memcpy(data->mini_map, tmp, \
		sizeof(*(data->mini_map)) * (size - LINEBUFFERSIZE));
	free(tmp);
}

void	ft_create_east(t_data *data, int32_t x, int32_t	y)
{
	int32_t	z;

	if (x + 1 < data->map_size.x && data->map[x + 1][y] != 1 && \
		(y - 1 < 0 || data->map[x][y - 1] != 1 || data->map[x + 1][y - 1] == 1))
	{
		z = 1;
		while (y + z < data->map_size.y && data->map[x][y + z] == 1 && \
				data->map[x + 1][y + z] != 1)
			z++;
		ft_realloc_linebuffer(data);
		data->mini_map[data->mm_size].a.x = x + 1;
		data->mini_map[data->mm_size].a.y = y;
		data->mini_map[data->mm_size].b.x = x + 1;
		data->mini_map[data->mm_size].b.y = y + z;
		data->mm_size++;
	}
}

void	ft_create_west(t_data *data, int32_t x, int32_t y)
{
	int32_t	z;

	if (x > 0 && data->map[x - 1][y] != 1 && (y - 1 < 0 || \
		data->map[x][y - 1] != 1 || data->map[x - 1][y - 1] == 1))
	{
		z = 1;
		while (y + z < data->map_size.y && data->map[x][y + z] == 1 && \
				data->map[x - 1][y + z] != 1)
			z++;
		ft_realloc_linebuffer(data);
		data->mini_map[data->mm_size].a.x = x;
		data->mini_map[data->mm_size].a.y = y;
		data->mini_map[data->mm_size].b.x = x;
		data->mini_map[data->mm_size].b.y = y + z;
		data->mm_size++;
	}
}

void	ft_create_south(t_data *data, int32_t x, int32_t y)
{
	int32_t	z;

	if (y > 0 && data->map[x][y - 1] != 1 && (x - 1 < 0 || \
		data->map[x - 1][y] != 1 || data->map[x - 1][y - 1] == 1))
	{
		z = 1;
		while (x + z < data->map_size.x && data->map[x + z][y] == 1 && \
				data->map[x + z][y - 1] != 1)
			z++;
		ft_realloc_linebuffer(data);
		data->mini_map[data->mm_size].a.x = x;
		data->mini_map[data->mm_size].a.y = y;
		data->mini_map[data->mm_size].b.x = x + z;
		data->mini_map[data->mm_size].b.y = y;
		data->mm_size++;
	}
}

void	ft_create_north(t_data *data, int32_t x, int32_t y)
{
	int32_t	z;

	if (y + 1 < data->map_size.y && data->map[x][y + 1] != 1 && \
		(x - 1 < 0 || data->map[x - 1][y] != 1 || data->map[x - 1][y + 1] == 1))
	{
		z = 1;
		while (x + z < data->map_size.x && data->map[x + z][y] == 1 && \
				data->map[x + z][y + 1] != 1)
			z++;
		ft_realloc_linebuffer(data);
		data->mini_map[data->mm_size].a.x = x;
		data->mini_map[data->mm_size].a.y = y + 1;
		data->mini_map[data->mm_size].b.x = x + z;
		data->mini_map[data->mm_size].b.y = y + 1;
		data->mm_size++;
	}
}

void	ft_create_door(t_data *data, int32_t x, int32_t y)
{
	if (y > 0 && data->map[x][y - 1] != 1 && y + 1 < data->map_size.y && \
		data->map[x][y + 1] != 1)
	{
		ft_realloc_linebuffer(data);
		data->mini_map[data->mm_size].a.x = x + 1;
		data->mini_map[data->mm_size].a.y = y + 0.5;
		data->mini_map[data->mm_size].b.x = x;
		data->mini_map[data->mm_size].b.y = y + 0.5;
		data->mm_size++;
	}
	if (x > 0 && data->map[x - 1][y] != 1 && x + 1 < data->map_size.x && \
		data->map[x + 1][y] != 1)
	{
		ft_realloc_linebuffer(data);
		data->mini_map[data->mm_size].a.x = x + 0.5;
		data->mini_map[data->mm_size].a.y = y + 1;
		data->mini_map[data->mm_size].b.x = x + 0.5;
		data->mini_map[data->mm_size].b.y = y;
		data->mm_size++;
	}
}

void	ft_create_minimap(t_data *data)
{
	int32_t	x;
	int32_t	y;

	x = 0;
	while (x < data->map_size.x)
	{
		y = 0;
		while (y < data->map_size.y)
		{
			if (data->map[x][y] == 1)
			{
				ft_create_east(data, x, y);
				ft_create_west(data, x, y);
				ft_create_north(data, x, y);
				ft_create_south(data, x, y);
			}
			else if (data->map[x][y] == 2)
				ft_create_door(data, x, y);
			y++;
		}
		x++;
	}
}

static void	draw_pixel(mlx_image_t *img, t_vec2i p, uint32_t col)
{
	const int32_t	i = (p.x + p.y * img->width);

	if (p.x >= 0 && p.x < (int32_t)img->width
		&& p.y >= 0 && p.y < (int32_t)img->height)
		((uint32_t *)img->pixels)[i] = col;
}

static int8_t	sign(int32_t a, int32_t b)
{
	if (a < b)
		return (1);
	else
		return (-1);
}

static void	draw_line(mlx_image_t *img, t_vec2i a, t_vec2i b, uint32_t col)
{
	const t_vec2i	d = {.x = abs(b.x - a.x), .y = abs(b.y - a.y) * -1};
	const t_vec2i	s = {.x = sign(a.x, b.x), .y = sign(a.y, b.y)};
	t_vec2i			e;

	e.x = d.x + d.y;
	while (1)
	{
		draw_pixel(img, a, col);
		e.y = 2 * e.x;
		if ((a.x == b.x && a.y == b.y)
			|| (e.y >= d.y && a.x == b.x) || (e.y <= d.x && a.y == b.y))
			return ;
		if (e.y >= d.y)
		{
			e.x += d.y;
			a.x += s.x;
		}
		if (e.y <= d.x)
		{
			e.x += d.x;
			a.y += s.y;
		}
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

	fill(data->mm_win, 0xAA000000);
	i = -1;
	while (i++ < data->mm_size)
	{
		af = data->mini_map[i].a;
		af.x -= data->pos.x;
		af.y -= data->pos.y;
		af = rotate_vec2f(af, -data->dir - PI);
		bf = data->mini_map[i].b;
		bf.x -= data->pos.x;
		bf.y -= data->pos.y;
		bf = rotate_vec2f(bf, -data->dir - PI);
		a.x = af.x * data->mm_scale + data->mm_win_h.x;
		a.y = af.y * data->mm_scale + data->mm_win_h.y;
		b.x = bf.x * data->mm_scale + data->mm_win_h.x;
		b.y = bf.y * data->mm_scale + data->mm_win_h.y;
		draw_line(data->mm_win, a, b, 0xFFFFFFFF);
	}
}
