/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:34:11 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/06 15:02:20 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	draw_rectangle(mlx_image_t *img, t_vec2i start, t_vec2i size, int col)
{
	int	x_temp;
	int	y_temp;

	size.y += start.y--;
	x_temp = start.x;
	while (++start.y < size.y)
	{
		y_temp = start.y * img->width;
		while (start.x < x_temp + size.x)
			((int *)img->pixels)[y_temp + start.x++] = col;
		start.x = x_temp;
	}
}

void	txt_to_img(mlx_image_t *dst, mlx_texture_t *src, \
	t_vec2i loc, float x_hit)
{
	int			index;
	t_vec2f		src_loc;
	int const	draw_height = dst->height - loc.y;
	float const	y_step = (float)src->width / (draw_height - loc.y);

	x_hit -= (int)x_hit;
	if (x_hit < 0)
		x_hit = 1 + x_hit;
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
		((int *)dst->pixels)[loc.y * dst->width + loc.x] \
			= ((int *)src->pixels)[index];
		src_loc.x += y_step;
		loc.y++;
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

void	draw_line(mlx_image_t *img, t_vec2i a, t_vec2i b, uint32_t col)
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
