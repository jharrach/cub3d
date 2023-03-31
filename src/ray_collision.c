/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_collision.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:30:56 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/30 19:45:26 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

// void	moving_door(t_ray *ray, t_data *data, int32_t i)
// {

// }

static bool	draw_door(t_data *data, float texture_x, int32_t cell, int32_t i)
{
	int32_t	j;

	j = ((int)data->win->height - data->ray_lenghts[i]) / 2;
	if (cell == 3)
	{
		if (texture_x < data->door.moved)
			return (false);
		txt_to_img(data->win, data->texture[1], (t_vec2i){i, j}, \
			texture_x - data->door.moved);
	}
	else
		txt_to_img(data->win, data->texture[1], (t_vec2i){i, j}, texture_x);
	return (true);
}

bool	door_collision(t_ray *ray, t_data *data, int32_t i)
{
	float	len;
	float	texture_x;

	if (ray->hoz && (!ray->cos || \
			ray->length.x - ray->unit.x / 2.0 < ray->length.y))
		len = ray->length.x - ray->unit.x / 2.0;
	else if (!ray->hoz && (!ray->sin || \
			ray->length.y - ray->unit.y / 2.0 <= ray->length.x))
		len = ray->length.y - ray->unit.y / 2.0;
	else
		return (false);
	if (ray->hoz)
		texture_x = data->pos.y + ray->cos * len;
	else
		texture_x = data->pos.x - ray->sin * len;
	texture_x -= (int)texture_x;
	data->ray_lenghts[i] = data->dis / (len * cosf(ray->angle));
	return (draw_door(data, texture_x, \
		data->map[ray->cell_pos.x][ray->cell_pos.y], i));
}

void	wall_collision(t_ray *ray, t_data *data, int32_t i)
{
	int	j;

	data->ray_lenghts[i] = data->dis / (ray->len * cosf(ray->angle));
	j = ((int)data->win->height - data->ray_lenghts[i]) / 2;
	if (ray->hoz && ray->cell_dir.x == 1)
		txt_to_img(data->win, data->texture[0], (t_vec2i){i, j}, \
			-(data->pos.y + ray->cos * ray->len));
	else if (ray->hoz)
		txt_to_img(data->win, data->texture[1], (t_vec2i){i, j}, \
			(data->pos.y + ray->cos * ray->len));
	else if (ray->cell_dir.y == 1)
		txt_to_img(data->win, data->texture[2], (t_vec2i){i, j}, \
			(data->pos.x - ray->sin * ray->len));
	else
		txt_to_img(data->win, data->texture[3], (t_vec2i){i, j}, \
			-(data->pos.x - ray->sin * ray->len));
}
