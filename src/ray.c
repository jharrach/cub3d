/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:17:22 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/02 22:26:09 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	ft_init_ray(t_ray *ray, t_data *data)
{
	if (ray->sin < 0.0)
	{
		ray->cell_dir.x = 1;
		ray->unit.x = 1.0 / fabsf(ray->sin);
		ray->length.x = ray->unit.x * (1.0 - (data->pos.x - ray->cell_pos.x));
	}
	else if (ray->sin > 0.0)
	{
		ray->cell_dir.x = -1;
		ray->unit.x = 1.0 / fabsf(ray->sin);
		ray->length.x = ray->unit.x * (data->pos.x - ray->cell_pos.x);
	}
	if (ray->cos > 0.0)
	{
		ray->cell_dir.y = 1;
		ray->unit.y = 1.0 / fabsf(ray->cos);
		ray->length.y = ray->unit.y * (1.0 - (data->pos.y - ray->cell_pos.y));
	}
	else if (ray->cos < 0.0)
	{
		ray->cell_dir.y = -1;
		ray->unit.y = 1.0 / fabsf(ray->cos);
		ray->length.y = ray->unit.y * (data->pos.y - ray->cell_pos.y);
	}
}

void	update_ray_angles(t_data *data)
{
	int32_t	i;

	i = 0;
	while (i < (int)data->win->width)
	{
		data->ray_angle[i] = atanf((float)(i - (int)data->win_wh) / data->dis);
		i++;
	}
}

void	ft_rays(t_data *data)
{
	int32_t	i;
	t_ray	ray;

	i = 0;
	while (i < (int)data->win->width)
	{
		ray.angle = data->ray_angle[i];
		ray.cos = cosf(data->dir + ray.angle);
		ray.sin = sinf(data->dir + ray.angle);
		ray.cell_pos.x = floorf(data->pos.x);
		ray.cell_pos.y = floorf(data->pos.y);
		ft_init_ray(&ray, data);
		ft_cast_ray(&ray, data, i);
		i++;
	}
}
