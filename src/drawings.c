/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:34:11 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/03 16:10:54 by jharrach         ###   ########.fr       */
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

// ART!!!!
// void	txt_to_img(mlx_image_t *dst, mlx_texture_t *src, \
// 	t_vec2i loc, float x_hit)
// {
// 	int		y_temp;
// 	int		index;
// 	t_vec2f	src_loc;
// 	int		draw_height;

// 	x_hit -= (int)x_hit;
// 	if (x_hit < 0)
// 		x_hit = 1 + x_hit;
// 	draw_height = dst->height - loc.y * 2;
// 	y_temp = loc.y;
// 	if (loc.y < 0)
// 		loc.y = 0;
// 	while (loc.y < y_temp + draw_height && loc.y < (int)dst->height)
// 	{
// 		src_loc.y = (float)src->width / draw_height * (loc.y - y_temp);
// 		src_loc.x = src->height * x_hit;
// 		index = (int)src_loc.y * src->height + src_loc.x;
// 		if (((int *)src->pixels)[index])
// 			((int *)dst->pixels)[loc.y * dst->width + loc.x] \
// 			= ((int *)src->pixels)[index];
// 		loc.y++;
// 	}
// }


// // ALSO ART
// void txt_to_img(mlx_image_t *dst, mlx_texture_t *src, t_vec2i loc, float x_hit)
// {
// 	int		index;
// 	t_vec2f	src_loc;
// 	int		draw_height;
// 	float	y_step;

// 	x_hit -= (int)x_hit;
// 	if (x_hit < 0)
// 		x_hit = 1 + x_hit;
// 	draw_height = dst->height - loc.y * 2;
// 	y_step = (float)src->height / draw_height;
// 	draw_height += loc.y;
// 	if (loc.y < 0)
// 	{
// 		src_loc.y = y_step * -loc.y;
// 		loc.y = 0;
// 	}
// 	else
// 		src_loc.y = 0;
// 	src_loc.x = src->width * x_hit;
// 	while (loc.y < draw_height && loc.y < (int)dst->height)
// 	{
// 		index = (int)src_loc.y * src->height + src_loc.x;
// 		if (((int *)src->pixels)[index])
// 			((int *)dst->pixels)[loc.y * dst->width + loc.x] = ((int *)src->pixels)[index];
// 		loc.y++;
// 		src_loc.y += y_step;
// 	}
// }

void	txt_to_img(mlx_image_t *dst, mlx_texture_t *src, \
	t_vec2i loc, float x_hit)
{
	int		index;
	t_vec2f	src_loc;
	int		draw_height;
	float	y_step;

	x_hit -= (int)x_hit;
	if (x_hit < 0)
		x_hit = 1 + x_hit;
	draw_height = dst->height - loc.y * 2;
	y_step = (float)src->height / draw_height;
	draw_height += loc.y;
	if (loc.y < 0)
	{
		src_loc.y = y_step * -loc.y;
		loc.y = 0;
	}
	else
		src_loc.y = 0;
	src_loc.x = src->width * x_hit;
	while (loc.y < draw_height && loc.y < (int)dst->height)
	{
		index = (int)src_loc.y * src->width + src_loc.x;
		if (((int *)src->pixels)[index])
			((int *)dst->pixels)[loc.y * dst->width + loc.x] \
			= ((int *)src->pixels)[index];
		src_loc.y += y_step;
		loc.y++;
	}
}

// void	txt_to_img(mlx_image_t *dst, mlx_texture_t *src, \
// 	t_vec2i loc, float x_hit)
// {
// 	int		y_temp;
// 	int		index;
// 	t_vec2f	src_loc;
// 	int		draw_height;

// 	x_hit -= (int)x_hit;
// 	if (x_hit < 0)
// 		x_hit = 1 + x_hit;
// 	draw_height = dst->height - loc.y * 2;
// 	y_temp = loc.y;
// 	if (loc.y < 0)
// 		loc.y = 0;
// 	while (loc.y < y_temp + draw_height && loc.y < (int)dst->height)
// 	{
// 		src_loc.y = (float)src->height / draw_height * (loc.y - y_temp);
// 		src_loc.x = src->width * x_hit;
// 		index = (int)src_loc.y * src->width + src_loc.x;
// 		if (((int *)src->pixels)[index])
// 			((int *)dst->pixels)[loc.y * dst->width + loc.x] \
// 			= ((int *)src->pixels)[index];
// 		loc.y++;
// 	}
// }
