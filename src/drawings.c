/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:34:11 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/02 22:28:55 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	draw_rectangle(mlx_image_t *img, t_vec2i start, t_vec2i size, int col)
{
	int	x_temp;

	size.y += start.y--;
	x_temp = start.x;
	while (++start.y < size.y)
	{
		while (start.x < x_temp + size.x)
			((int *)img->pixels)[start.y * img->width + start.x++] = col;
		start.x = x_temp;
	}
}

int32_t	factor_pixel(int c, float f)
{
	if (f > 1.0)
		return (0xFF000000);
	return (255 << 24 | (int)((c >> 16 & 0xFF) * f) << 16 \
		| (int)((c >> 8 & 0xFF) * f) << 8
		| (int)((c & 0xFF) * f));
}

void	txt_to_img(mlx_image_t *dst, mlx_texture_t *src, \
	t_vec2i loc, float x_hit)
{
	int		y_temp;
	int		index;
	t_vec2f	src_loc;
	int		draw_height;

	x_hit -= (int)x_hit;
	if (x_hit < 0)
		x_hit = 1 + x_hit;
	draw_height = dst->height - loc.y * 2;
	y_temp = loc.y;
	if (loc.y < 0)
		loc.y = 0;
	while (loc.y < y_temp + draw_height && loc.y < (int)dst->height)
	{
		src_loc.y = (float)src->height / draw_height * (loc.y - y_temp);
		src_loc.x = src->width * x_hit;
		index = (int)src_loc.y * src->width + src_loc.x;
		if (((int *)src->pixels)[index])
			((int *)dst->pixels)[loc.y * dst->width + loc.x] \
			= factor_pixel(((int *)src->pixels)[index], 1);
		loc.y++;
	}
}
