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

float	texture_width(mlx_texture_t *tex)
{
	uint32_t	i;
	uint32_t	j;
	bool		tex_started;

	tex_started = false;
	i = 0;
	while (i < tex->width && !tex_started)
	{
		j = 0;
		while (j < tex->height && !tex_started)
		{
			if (((uint32_t *)tex->pixels)[j * tex->width + i])
				tex_started = true;
			if (((uint32_t *)tex->pixels)[j * tex->width + tex->width - i - 1])
				tex_started = true;
			j++;
		}
		i++;
	}
	return ((float)(tex->width - i) / ((float)tex->width * 2.0));
}

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

void	scale_texture_to_img(mlx_texture_t *texture, mlx_image_t *image)
{
	uint32_t	i;
	uint32_t	j;
	float const	w = (float)texture->width / (float)image->width;
	float const	h = (float)texture->height / (float)image->height;

	i = 0;
	while (i < image->height)
	{
		j = 0;
		while (j < image->width)
		{
			((int32_t *)image->pixels)[i * image->width + j] = \
				((int32_t *)texture->pixels) \
				[(int32_t)(i * h) * texture->width + (int32_t)(j * w)];
			j++;
		}
		i++;
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
	data->mm_win = mlx_new_image(data->mlx, data->win->width / 8, data->win->width / 8);
	if (!data->win || !data->mm_win || !data->win_entities)
		destroy_data(data, 1, "Failed to create MLX image!");

	//new
	data->mm_win_h.x = data->mm_win->width / 2;
	data->mm_win_h.y = data->mm_win->height / 2;
	data->mm_img = mlx_new_image(data->mlx, data->win->width / 8, data->win->width / 8);
	if (data->mm_img == NULL)
		destroy_data(data, 1, "Failed to create MLX image!");
}

void	remove_transparency(uint8_t *pixels, uint32_t width, uint32_t height)
{
	uint32_t		i;
	uint32_t		j;
	uint32_t *const	p = (uint32_t *)pixels;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			if (p[i * width + j])
				p[i * width + j] |= 0xFF000000;
			j++;
		}
		i++;
	}
}

mlx_texture_t	*rotate_texture(mlx_texture_t *texture)
{
	mlx_texture_t	*rotated;
	uint32_t		i;
	uint32_t		j;

	rotated = malloc(sizeof(*rotated));
	rotated->pixels = malloc(sizeof(*(rotated->pixels)) * texture->width * texture->height * texture->bytes_per_pixel);
	if (!rotated->pixels)
		fprintf(stderr, "I have expected all of this, you know\n");
	rotated->width = texture->height;
	rotated->height = texture->width;
	rotated->bytes_per_pixel = texture->bytes_per_pixel;
	i = 0;
	while (i < rotated->height)
	{
		j = 0;
		while (j < rotated->width)
		{
			((uint32_t *)rotated->pixels)[i * rotated->width + j] = ((uint32_t *)texture->pixels)[j * texture->width + i];
			j++;
		}
		i++;
	}
	mlx_delete_texture(texture);
	return (rotated);
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
	init_mlx(data);
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	data->ray_angle = ft_alloc(data, data->win->width, sizeof(*(data->ray_angle)));
	data->ray_lenghts = ft_alloc(data, data->win->width, sizeof(*(data->ray_lenghts)));
	data->mm_scale = (float)data->win->width / 50.0;
	update_ray_angles(data);//
	load_data(data, &data->in);
	remove_transparency(data->texture[5]->pixels, data->texture[5]->width, data->texture[5]->height);
	for (int i = 0; i < BG; i++)
		data->texture[i] = rotate_texture(data->texture[i]);
	init_entities(data);
	ft_create_minimap(data);
}
