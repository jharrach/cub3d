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
				data->entity[i].img = data->texture[5];
				data->entity[i].half_width = 0.5;//will be replaced with function
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
	data->win_entities = \
		mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	data->mm_win = mlx_new_image(data->mlx, 200, 200);
	if (!data->win || !data->mm_win || !data->win_entities)
		destroy_data(data, 1, "Failed to create MLX image!");

	//new
	data->mm_win_h.x = data->mm_win->width / 2;
	data->mm_win_h.y = data->mm_win->height / 2;
	mlx_texture_t	*txt = mlx_load_png("textures/minimap.png");
	if (!txt)
		exit(42);
	data->mm_txt = mlx_texture_to_image(data->mlx, txt);
	mlx_delete_texture(txt);
	if (data->mm_txt == NULL)
		destroy_data(data, 1, "Failed to create MLX image!");
}

void	init_data(t_data *data, char *fn)
{
	*data = (t_data){0};
	data->in.fd = -1;
	data->fov = FOV * PI / 180.0;
	data->pos = (t_vec2f){-1, -1};
	data->door.moved = -1.0;
	if (ft_strlen(fn) < 5 || ft_strcmp(fn + ft_strlen(fn) - 4, ".cub"))
		destroy_data(data, 1, "Invalid filename type!");
	data->in.fd = open(fn, O_RDONLY);
	if (data->in.fd == -1)
		destroy_data(data, 1, "Failed to open input file!");
	init_mlx(data);
	data->dir_delta = 0.0 * PI / 180.0;
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	data->ray_angle = ft_alloc(data, data->win->width, sizeof(*(data->ray_angle)));
	data->ray_lenghts = ft_alloc(data, data->win->width, sizeof(*(data->ray_lenghts)));
	update_ray_angles(data);//
	load_data(data, &data->in);
	init_entities(data);
	ft_create_minimap(data);
	//Debug destroy
		destroy_data(data, 1, "Debug destroy");
}
