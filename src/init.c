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

void	init_data(t_data *data, char *fn)
{
	data->ray_lenghts = NULL;
	ft_memset(data->texture, 0, sizeof(data->texture));
	data->fov = FOV * PI / 180.0;
	data->pos = (t_vec2f){.x = -2.0f, .y = -2.0f};
	data->door.moved = -1.0;
	data->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (data->mlx == NULL)
		destroy_data(data, &data->in, 1, "Failed to init MLX window!");
	if (ft_strlen(fn) < 5 || ft_strcmp(fn + ft_strlen(fn) - 4, ".cub"))
		destroy_data(data, &data->in, 1, "Invalid filename type!");
	data->in.fd = open(fn, O_RDONLY);
	if (data->in.fd == -1)
		destroy_data(data, &data->in, 1, "Failed to open input file!");
	mlx_set_window_limit(data->mlx, 160, 90, __INT_MAX__, __INT_MAX__);
	data->win = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	if (data->win == NULL)
		destroy_data(data, &data->in, 1, "Failed to create MLX image!");
	data->win_entities = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	if (data->win_entities == NULL)
		destroy_data(data, &data->in, 1, "Failed to create MLX image!");
	data->ray_lenghts = malloc(sizeof(*(data->ray_lenghts)) * data->win->width);
	if (data->ray_lenghts == NULL)
		destroy_data(data, &data->in, 1, "Failed to allocate depth buffer!");
	data->dir_delta = 0.0 * PI / 180.0;
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	load_data(data, &data->in);
	data->num_entities = 2;
	data->entity = malloc(sizeof(*(data->entity)) * data->num_entities);
	data->entity[0].pos = (t_vec2f){.x = -2.0, .y = -2.0};
	data->entity[0].enabled = true;
	data->entity[0].img = data->texture[5];
	data->entity[1].pos = (t_vec2f){.x = -2.0, .y = -4.0};
	data->entity[1].enabled = true;
	data->entity[1].img = data->texture[5];
}
