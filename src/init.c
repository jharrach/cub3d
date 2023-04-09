/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 13:58:05 by rburgsta          #+#    #+#             */
/*   Updated: 2023/04/01 13:58:05 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	init_entities(t_data *data)
{
	int	y;
	int	x;
	int	i;

	data->entity = ft_alloc(data, data->num_entities, sizeof(*(data->entity)));
	x = -1;
	i = -1;
	while (++x < data->map_size.x)
	{
		y = -1;
		while (++y < data->map_size.y)
		{
			if (data->map[x][y] == 3)
			{
				data->entity[++i].pos = (t_vec2f){x + 0.5, y + 0.5};
				data->entity[i].enabled = true;
				data->entity[i].half_width = texture_width(data->texture[5]);
				init_rectf_center_vec2f(&(data->entity[i].rect), \
					data->entity[i].pos, data->entity[i].half_width);
				data->map[x][y] = 0;
			}
		}
	}
}

static void	init_mlx(t_data *data)
{
	data->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (data->mlx == NULL)
		destroy_data(data, 1, "Failed to init MLX window!");
	mlx_set_window_limit(data->mlx, 160, 90, 1920, 1080);
	data->win = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	data->win_entities = \
		mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	data->mm.win = mlx_new_image(data->mlx, \
		data->win->width / 8, data->win->width / 8);
	if (!data->win || !data->mm.win || !data->win_entities)
		destroy_data(data, 1, "Failed to create MLX image!");
	data->mm.win_h.x = (float)data->mm.win->width / 2.0;
	data->mm.win_h.y = (float)data->mm.win->height / 2.0;
	data->mm.win_tex = mlx_new_image(data->mlx, data->win->width / 8, \
		data->win->width / 8);
	if (data->mm.win_tex == NULL)
		destroy_data(data, 1, "Failed to create MLX image!");
}

void	init_data(t_data *data, char *fn)
{
	*data = (t_data){0};
	data->in.fd = -1;
	data->menu = true;
	data->fov = FOV * PI / 180.0;
	data->pos = (t_vec2f){-1, -1};
	data->door.moved = -1.0;
	if (ft_strlen(fn) < 5 || ft_strcmp(fn + ft_strlen(fn) - 4, ".cub"))
		destroy_data(data, 1, "Invalid filename type!");
	data->in.fd = open(fn, O_RDONLY);
	if (data->in.fd == -1)
		destroy_data(data, 1, "Failed to open input file!");
	load_data(data, &data->in);
	init_mlx(data);
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	data->ray_angle = ft_alloc(data, data->win->width, \
		sizeof(*(data->ray_angle)));
	data->ray_lenghts = ft_alloc(data, data->win->width, \
		sizeof(*(data->ray_lenghts)));
	data->mm.scale = (float)data->win->width / MM_SCALE;
	update_ray_angles(data);
	texture_processing(data);
	init_entities(data);
	ft_create_minimap(data);
}
