/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 01:35:05 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/12 01:19:36 by jharrach         ###   ########.fr       */
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

static mlx_texture_t	*rotate_texture(mlx_texture_t *texture)
{
	uint32_t		*pixel;
	uint32_t		tmp;
	t_vec2i			i;

	pixel = malloc(sizeof(*(texture->pixels)) * texture->width * \
		texture->height * texture->bytes_per_pixel);
	if (!pixel)
	{
		mlx_delete_texture(texture);
		return (NULL);
	}
	tmp = texture->width;
	texture->width = texture->height;
	texture->height = tmp;
	i.y = -1;
	while (++i.y < (int32_t)texture->height)
	{
		i.x = -1;
		while (++i.x < (int32_t)texture->width)
			pixel[i.y * texture->width + i.x] = ((uint32_t *)texture->pixels) \
				[i.x * tmp + (texture->height - i.y - 1)];
	}
	free(texture->pixels);
	texture->pixels = (uint8_t *)pixel;
	return (texture);
}

static void	remove_transparency(uint8_t *pixels, \
	uint32_t width, uint32_t height)
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

void	texture_processing(t_data *data)
{
	int32_t	i;

	i = 0;
	while (i < B1)
	{
		if (i < BG)
		{
			data->texture[i] = rotate_texture(data->texture[i]);
			if (!data->texture[i])
				destroy_data(data, true, "Failed to allocate memory!");
		}
		remove_transparency(data->texture[i]->pixels, \
			data->texture[i]->width, data->texture[i]->height);
		i++;
	}
}
