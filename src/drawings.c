/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:34:11 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/05 19:23:02 by jharrach         ###   ########.fr       */
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
