/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:17:22 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/30 19:04:49 by jharrach         ###   ########.fr       */
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

static void	ft_cmp_ray_lenghts(t_ray *ray)
{
	if (ray->sin != 0.0 && (ray->cos == 0.0 || ray->length.x < ray->length.y))
	{
		ray->len = ray->length.x;
		ray->cell_pos.x += ray->cell_dir.x;
		ray->length.x += ray->unit.x;
		ray->hoz = true;
	}
	else
	{
		ray->len = ray->length.y;
		ray->cell_pos.y += ray->cell_dir.y;
		ray->length.y += ray->unit.y;
		ray->hoz = false;
	}
}

static void	ft_cast_ray(t_ray *ray, t_data *data, int32_t i)
{
	while (1)
	{
		ft_cmp_ray_lenghts(ray);
		if (ray->len > 50.0)
		{
			data->ray_lenghts[i] = 0.0;
			break ;
		}
		if (ray->len > 0.05 && \
			ray->cell_pos.x >= 0 && ray->cell_pos.x < data->map_size.x && \
			ray->cell_pos.y >= 0 && ray->cell_pos.y < data->map_size.y)
		{
			if (data->map[ray->cell_pos.x][ray->cell_pos.y] == 1)
			{
				wall_collision(ray, data, i);
				return ;
			}
			else if (data->map[ray->cell_pos.x][ray->cell_pos.y] == 2 || \
				data->map[ray->cell_pos.x][ray->cell_pos.y] == 3)
			{
				if (door_collision(ray, data, i))
					return ;
			}
		}
	}
}

void	ft_rays(t_data *data)
{
	int32_t	i;
	t_ray	ray;

	i = 0;
	while (i < (int)data->win->width)
	{
		ray.angle = atanf((float)(i - (int)data->win_wh) / data->dis);
		ray.cos = cosf(data->dir + ray.angle);
		ray.sin = sinf(data->dir + ray.angle);
		ray.cell_pos.x = floorf(data->pos.x);
		ray.cell_pos.y = floorf(data->pos.y);
		ft_init_ray(&ray, data);
		ft_cast_ray(&ray, data, i);
		i++;
	}
}
