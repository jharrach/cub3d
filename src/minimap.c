/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 21:16:53 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/09 02:36:04 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	ft_create_east(t_data *data, int32_t x, int32_t	y)
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
		data->mm.line[data->mm.num_lines].a.x = x + 1;
		data->mm.line[data->mm.num_lines].a.y = y;
		data->mm.line[data->mm.num_lines].b.x = x + 1;
		data->mm.line[data->mm.num_lines].b.y = y + z;
		data->mm.num_lines++;
	}
}

static void	ft_create_west(t_data *data, int32_t x, int32_t y)
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
		data->mm.line[data->mm.num_lines].a.x = x;
		data->mm.line[data->mm.num_lines].a.y = y;
		data->mm.line[data->mm.num_lines].b.x = x;
		data->mm.line[data->mm.num_lines].b.y = y + z;
		data->mm.num_lines++;
	}
}

static void	ft_create_south(t_data *data, int32_t x, int32_t y)
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
		data->mm.line[data->mm.num_lines].a.x = x;
		data->mm.line[data->mm.num_lines].a.y = y;
		data->mm.line[data->mm.num_lines].b.x = x + z;
		data->mm.line[data->mm.num_lines].b.y = y;
		data->mm.num_lines++;
	}
}

static void	ft_create_north(t_data *data, int32_t x, int32_t y)
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
		data->mm.line[data->mm.num_lines].a.x = x;
		data->mm.line[data->mm.num_lines].a.y = y + 1;
		data->mm.line[data->mm.num_lines].b.x = x + z;
		data->mm.line[data->mm.num_lines].b.y = y + 1;
		data->mm.num_lines++;
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
	scale_texture_to_img(data->texture[MM], data->mm.win_tex);
}
