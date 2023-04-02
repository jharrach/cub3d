/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:02:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/02 16:02:06 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_fps(t_data *data, int32_t x, int32_t y)
{
	static mlx_image_t	*img = NULL;
	char				*str;

	str = ft_itoa(1.0 / data->mlx->delta_time);
	if (!str)
		destroy_data(data, 1, "ft_fps itoa fail");
	mlx_delete_image(data->mlx, img);
	img = mlx_put_string(data->mlx, str, x, y);
	free(str);
	if (!img)
		destroy_data(data, 1, "ft_fps putstring fail");
}

void	init_recti_center_vec2f(t_recti *rect, t_vec2f center, float halfwidth)
{
	rect->tr.x = floorf(center.x + halfwidth);
	rect->tr.y = floorf(center.y + halfwidth);
	rect->tl.x = floorf(center.x - halfwidth);
	rect->tl.y = floorf(center.y + halfwidth);
	rect->br.x = floorf(center.x + halfwidth);
	rect->br.y = floorf(center.y - halfwidth);
	rect->bl.x = floorf(center.x - halfwidth);
	rect->bl.y = floorf(center.y - halfwidth);
}

bool	vec2i_in_range(t_vec2i v, t_vec2i max)
{
	if (v.x < 0 || v.x >= max.x)
		return (false);
	if (v.y < 0 || v.y >= max.y)
		return (false);
	return (true);
}

bool	recti_collide_map(t_recti *rect, int **map, t_vec2i map_size)
{
	if (vec2i_in_range(rect->tr, map_size) && map[rect->tr.x][rect->tr.y] > 0)
		return (true);
	if (vec2i_in_range(rect->tl, map_size) && map[rect->tl.x][rect->tl.y] > 0)
		return (true);
	if (vec2i_in_range(rect->br, map_size) && map[rect->br.x][rect->br.y] > 0)
		return (true);
	if (vec2i_in_range(rect->bl, map_size) && map[rect->bl.x][rect->bl.y] > 0)
		return (true);
	return (false);
}

t_vec2f	vec2f_mul_f(t_vec2f v, float m)
{
	v.x *= m;
	v.y *= m;
	return (v);
}

t_vec2f	vec2f_add(t_vec2f v1, t_vec2f v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	return (v1);
}

t_vec2f	vec2f_sub(t_vec2f v1, t_vec2f v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	return (v1);
}

void	move_door(t_data *data)
{
	if (data->door.moved >= 0.0)
	{
		if (data->door.opens)
		{
			data->door.moved += data->mlx->delta_time;
			if (data->door.moved > 1.0)
			{
				data->door.moved = -1.0;
				data->map[data->door.cell.x][data->door.cell.y] = -1;
			}
		}
		else
		{
			data->door.moved -= data->mlx->delta_time;
			if (data->door.moved < 0.0)
			{
				data->door.moved = -1.0;
				data->map[data->door.cell.x][data->door.cell.y] = 2;
			}
		}
	}
}

void	get_mouse_input(t_data *data)
{
	int32_t	x;
	int32_t	y;

	mlx_get_mouse_pos(data->mlx, &x, &y);
	(void)y;
	data->dir = data->dir_delta + x * 0.001;
}

t_vec2f	rotate_vec2f(t_vec2f v1, float angle)
{
	t_vec2f	v2;

	v2.x = v1.x * cosf(angle) - v1.y * sinf(angle);
	v2.y = v1.x * sinf(angle) + v1.y * cosf(angle);
	return (v2);
}

static int	partition(t_entity *arr, int i, int end)
{
	int			pivotindex;
	float		pivotvalue;
	t_entity	tmp;

	pivotindex = i;
	pivotvalue = arr[end].del_pos.y;
	while (i < end)
	{
		if (arr[i].del_pos.y > pivotvalue)
		{
			tmp = arr[i];
			arr[i] = arr[pivotindex];
			arr[pivotindex] = tmp;
			pivotindex++;
		}
		i++;
	}
	tmp = arr[pivotindex];
	arr[pivotindex] = arr[end];
	arr[end] = tmp;
	return (pivotindex);
}

void	quick_sort_entities(t_entity *arr, int start, int end)
{
	int	i;

	if (start >= end)
		return ;
	i = partition(arr, start, end);
	quick_sort_entities(arr, start, i - 1);
	quick_sort_entities(arr, i + 1, end);
}

void	update_entitiy_del_pos(t_data *data)
{
	int32_t	i;

	i = 0;
	while (i < data->num_entities)
	{
		data->entity[i].del_pos = \
			vec2f_sub(data->entity[i].pos, data->pos);
		data->entity[i].del_pos.x *= -1;
		data->entity[i].del_pos = \
			rotate_vec2f(data->entity[i].del_pos, data->dir);
		i++;
	}
}

static void	draw_entity(t_entity *entity, t_data *data)
{
	t_vec2i	loc;
	int32_t	j;

	loc.x = entity->del_pos.x * (data->dis / entity->del_pos.y);
	loc.x += data->win_wh;
	entity->del_pos.y = data->dis / entity->del_pos.y;
	loc.x -= entity->del_pos.y / 2.0;
	j = 0;
	while (j < entity->del_pos.y)
	{
		if (loc.x >= 0 && loc.x < (int)data->win->width && \
			(entity->del_pos.y > data->ray_lenghts[loc.x] || \
				data->ray_lenghts[loc.x] == 0.0))
		{
			loc.y = ((int)data->win->height - entity->del_pos.y) / 2;
			txt_to_img(data->win_entities, entity->img, \
				loc, j / entity->del_pos.y);
		}
		loc.x++;
		j++;
	}
}

void	draw_entities(t_data *data)
{
	int32_t	i;

	update_entitiy_del_pos(data);
	quick_sort_entities(data->entity, 0, data->num_entities - 1);
	ft_memset(data->win_entities->pixels, 0, \
		data->win->width * data->win->height * 4);
	i = 0;
	while (i < data->num_entities)
	{
		if (data->entity[i].enabled && data->entity[i].del_pos.y > 0.05)
			draw_entity(&(data->entity[i]), data);
		i++;
	}
}

static t_vec2f	player_delta(mlx_t *mlx)
{
	t_vec2f	delta;

	delta = (t_vec2f){.x = 0.0, .y = 0.0};
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_W))
		delta.y += mlx->delta_time;
	if (mlx_is_key_down(mlx, MLX_KEY_S))
		delta.y -= mlx->delta_time;
	if (mlx_is_key_down(mlx, MLX_KEY_A))
		delta.x += mlx->delta_time;
	if (mlx_is_key_down(mlx, MLX_KEY_D))
		delta.x -= mlx->delta_time;
	if (delta.x && delta.y)
		delta = vec2f_mul_f(delta, 0.70710678118f);
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT_SHIFT))
		delta = vec2f_mul_f(delta, SHIFT_MULTIPLIER);
	else if (mlx_is_key_down(mlx, MLX_KEY_LEFT_CONTROL))
		delta = vec2f_mul_f(delta, CONTROL_MULTIPLIER);
	return (delta);
}

void	get_key_input(t_data *data)
{
	t_vec2f	delta;
	t_recti	rect;

	delta = rotate_vec2f(player_delta(data->mlx), data->dir);
	init_recti_center_vec2f(&rect, \
		(t_vec2f){data->pos.x + delta.x, data->pos.y}, PLAYER_HALF_WIDTH);
	if (!recti_collide_map(&rect, data->map, data->map_size))
		data->pos.x += delta.x;
	init_recti_center_vec2f(&rect, \
		(t_vec2f){data->pos.x, data->pos.y + delta.y}, PLAYER_HALF_WIDTH);
	if (!recti_collide_map(&rect, data->map, data->map_size))
		data->pos.y += delta.y;
}

void	ft_hook(void *param)
{
	t_data *const	data = param;

	ft_fps(data, 0, 0);
	get_mouse_input(data);
	get_key_input(data);
	draw_rectangle(data->win, 0, 0, \
		data->win->width, data->win->height / 2, data->col_ceiling);
	draw_rectangle(data->win, 0, data->win->height / 2, \
		data->win->width, data->win->height / 2, data->col_floor);
	ft_rays(data);
	draw_entities(data);
	move_door(data);
}

void	scroll(double xdelta, double ydelta, void *param)
{
	t_data *const	data = param;

	(void)(xdelta);
	data->fov += ydelta * 0.01;
	if (data->fov < PI / 6.0 || data->fov > PI * 2.0 / 3.0)
		data->fov -= ydelta * 0.01;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
}

void	ft_resize_hook(int32_t width, int32_t height, void *param)
{
	t_data *const	data = param;

	if (!mlx_resize_image(data->win, width, height))
		printf("error\n");
	if (!mlx_resize_image(data->win_entities, width, height))
		printf("error\n");
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	free(data->ray_lenghts);
	data->ray_lenghts = malloc(sizeof(*(data->ray_lenghts)) * data->win->width);
}

void	ft_keyhook(mlx_key_data_t keydata, void *param)
{
	t_data *const	data = param;
	static int		mode;
	t_vec2i			next_cell;
	t_recti			rect;

	if (keydata.key == MLX_KEY_Q && keydata.action == MLX_RELEASE)
	{
		mode = !mode;
		if (mode)
			mlx_set_cursor_mode(data->mlx, MLX_MOUSE_NORMAL);
		else
		{
			mlx_set_cursor_mode(data->mlx, MLX_MOUSE_DISABLED);
			mlx_focus(data->mlx);
		}
	}
	if (keydata.key == MLX_KEY_F && keydata.action == MLX_RELEASE)
	{
		next_cell.x = floorf(data->pos.x - sinf(data->dir));
		next_cell.y = floorf(data->pos.y + cosf(data->dir));
		if (vec2i_in_range(next_cell, data->map_size))
		{
			if (data->map[next_cell.x][next_cell.y] == 3)
				data->door.opens = !data->door.opens;
			else if (data->door.moved >= 0.0)
				return ;
			else if (data->map[next_cell.x][next_cell.y] == 2)
			{
				data->map[next_cell.x][next_cell.y] = 3;
				data->door.cell = next_cell;
				data->door.moved = 0.0;
				data->door.opens = true;
			}
			else if (data->map[next_cell.x][next_cell.y] == -1)
			{
				data->map[next_cell.x][next_cell.y] = 3;
				init_recti_center_vec2f(&rect, data->pos, 0.25);
				if (recti_collide_map(&rect, data->map, data->map_size))
				{
					data->map[next_cell.x][next_cell.y] = -1;
					return ;
				}
				data->door.cell = next_cell;
				data->door.moved = 1.0;
				data->door.opens = false;
			}
		}
	}
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
		return (printf("Error\nWrong number of arguments!\n"), 1);
	data.in.fd = -1;
	data.in.input = NULL;
	init_data(&data, argv[1]);
	mlx_set_cursor_mode(data.mlx, MLX_MOUSE_DISABLED);
	mlx_focus(data.mlx);
	if (mlx_image_to_window(data.mlx, data.win, 0, 0) == -1)
		destroy_data(&data, 1, "Failed to draw img to window");
	if (mlx_image_to_window(data.mlx, data.win_entities, 0, 0) == -1)
		destroy_data(&data, 1, "Failed to draw img to window");
	mlx_loop_hook(data.mlx, ft_hook, &data);
	mlx_scroll_hook(data.mlx, scroll, &data);
	mlx_key_hook(data.mlx, ft_keyhook, &data);
	mlx_resize_hook(data.mlx, ft_resize_hook, &data);
	mlx_loop(data.mlx);
	destroy_data(&data, 0, NULL);
	return (0);
}
