/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   button.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 17:21:45 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/08 17:46:15 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static bool	ft_button(mlx_image_t *win, mlx_texture_t *tex, t_vec2i mouse)
{
	t_vec2i			i;
	t_vec2i const	d = {
		.x = (int32_t)(win->width - tex->width) / 2,
		.y = (int32_t)(win->height - tex->height) / 2};
	bool			col;

	col = false;
	i.y = -1;
	while (++i.y < (int)tex->height)
	{
		i.x = -1;
		while (++i.x < (int)tex->width)
		{
			if (d.x + i.x >= 0 && d.x + i.x < (int32_t)win->width && \
				d.y + i.y >= 0 && d.y + i.y < (int32_t)win->height)
			{
				((uint32_t *)win->pixels)[(d.y + i.y) * win->width + d.x + \
					i.x] = ((uint32_t *)tex->pixels)[i.y * tex->width + i.x];
				if (d.x + i.x == mouse.x && d.y + i.y == mouse.y)
					col = true;
			}
		}
	}
	return (col);
}

static void	fill(mlx_image_t *img, uint32_t col)
{
	int32_t	i;

	i = img->width * img->height;
	while (i--)
		((uint32_t *)img->pixels)[i] = col;
}

static bool	ft_start_or_conitinue(t_data *data, bool col, t_vec2i mouse)
{
	if (data->started)
		col = ft_button(data->win_entities, data->texture[B3 + col], mouse);
	else
		col = ft_button(data->win_entities, data->texture[B1 + col], mouse);
	if (col && mlx_is_mouse_down(data->mlx, MLX_MOUSE_BUTTON_LEFT))
	{
		data->menu = false;
		data->mm_img->enabled = true;
		data->mm_win->enabled = true;
		data->started = true;
		data->dir_delta = mouse.x * -MOUSE_MUL;
		mlx_set_cursor_mode(data->mlx, MLX_MOUSE_DISABLED);
	}
	return (col);
}

void	ft_menu(t_data *data)
{
	static bool	col = false;
	t_vec2i		mouse;

	mlx_get_mouse_pos(data->mlx, &mouse.x, &mouse.y);
	data->mm_img->enabled = false;
	data->mm_win->enabled = false;
	scale_texture_to_img(data->texture[BG], data->win);
	fill(data->win_entities, 0x0);
	if (data->collected == data->num_entities)
	{
		col = ft_button(data->win_entities, data->texture[B5 + col], mouse);
		if (col && mlx_is_mouse_down(data->mlx, MLX_MOUSE_BUTTON_LEFT))
			mlx_close_window(data->mlx);
	}
	else
		col = ft_start_or_conitinue(data, col, mouse);
}
