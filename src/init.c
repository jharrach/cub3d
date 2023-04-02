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

	data->entity = malloc(sizeof(*(data->entity)) * data->num_entities);
	if (data->entity == NULL)
		destroy_data(data, 1, "Failed to allocate entities!");
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
				data->entity[i].img = data->texture[5];
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
	mlx_set_window_limit(data->mlx, 160, 90, __INT_MAX__, __INT_MAX__);
	data->win = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	if (data->win == NULL)
		destroy_data(data, 1, "Failed to create MLX image!");
	data->win_entities = \
		mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	if (data->win_entities == NULL)
		destroy_data(data, 1, "Failed to create MLX image!");
}

void	init_data(t_data *data, char *fn)
{
	data->mlx = NULL;
	data->ray_lenghts = NULL;
	data->map = NULL;
	data->entity = NULL;
	data->fov = FOV * PI / 180.0;
	data->pos = (t_vec2f){-1, -1};
	data->door.moved = -1.0;
	data->num_entities = 0;
	ft_memset(data->texture, 0, sizeof(data->texture));
	if (ft_strlen(fn) < 5 || ft_strcmp(fn + ft_strlen(fn) - 4, ".cub"))
		destroy_data(data, 1, "Invalid filename type!");
	data->in.fd = open(fn, O_RDONLY);
	if (data->in.fd == -1)
		destroy_data(data, 1, "Failed to open input file!");
	init_mlx(data);
	data->ray_lenghts = malloc(sizeof(*(data->ray_lenghts)) * data->win->width);
	if (data->ray_lenghts == NULL)
		destroy_data(data, 1, "Failed to allocate depth buffer!");
	data->dir_delta = 0.0 * PI / 180.0;
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	load_data(data, &data->in);
	init_entities(data);
}
