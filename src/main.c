/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:02:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/30 19:44:46 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_vec2f	npc;

void	ft_fps(mlx_t *mlx, int32_t x, int32_t y)
{
	static mlx_image_t	*img = NULL;
	char				*str;

	str = ft_itoa(1.0 / mlx->delta_time);
	if (!str)
		;//error
	mlx_delete_image(mlx, img);
	img = mlx_put_string(mlx, str, x, y);
	free(str);
	if (!img)
		;//error
}

void ft_hook(void* param)
{
	t_data *const	data = param;

	ft_fps(data->mlx, 0, 0);
	draw_rectangle(data->win, 0, 0, data->win->width, data->win->height / 2, 0xFF00FF00);
	draw_rectangle(data->win, 0, data->win->height / 2, data->win->width, data->win->height / 2, 0xFFFF0000);
	ft_rays(data);
	t_vec2f	npc_r;
	t_vec2f	npc_d;
	npc_d.x = npc.x - data->pos.x;
	npc_d.y = npc.y - data->pos.y;
	npc_d.x *= -1;
	npc_r.x = npc_d.x * cosf(data->dir) - npc_d.y * sinf(data->dir);
	npc_r.y = npc_d.x * sinf(data->dir) + npc_d.y * cosf(data->dir);
	if (npc_r.y > 0.05)
	{
		int32_t npc_x;
		npc_x = npc_r.x * (data->dis / npc_r.y);
		npc_x += data->win_wh;
		float hit;
		npc_r.y = data->dis / npc_r.y;//npc_r.y > 0.05
		int32_t	i = -npc_r.y / 2.0;
		while (i < (npc_r.y) / 2.0)
		{
			if (npc_x + i >= 0 && npc_x + i < (int)data->win->width && (npc_r.y > data->ray_lenghts[npc_x + i] || data->ray_lenghts[npc_x + i] == 0.0))
			{
				int	j = ((int)data->win->height - npc_r.y) / 2;
				hit = (i + npc_r.y / 2) / npc_r.y;
				txt_to_img(data->win, data->texture[0], (t_vec2i){npc_x + i, j}, hit);
			}
			i++;
		}
	}

	t_vec2f	move = {.x = 0.0, .y = 0.0};
	t_vec2f	delta;
	t_vec2i pos;
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
		move.y += data->mlx->delta_time;
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
		move.y -= data->mlx->delta_time;
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		move.x += data->mlx->delta_time;
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		move.x -= data->mlx->delta_time;
	if (mlx_is_key_down(data->mlx, MLX_KEY_UP))
		npc.y += data->mlx->delta_time;
	if (mlx_is_key_down(data->mlx, MLX_KEY_DOWN))
		npc.y -= data->mlx->delta_time;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		npc.x += data->mlx->delta_time;
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		npc.x -= data->mlx->delta_time;
	delta.x = move.x * cosf(data->dir) - move.y * sinf(data->dir);
	delta.y = move.x * sinf(data->dir) + move.y * cosf(data->dir);
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT_SHIFT))
	{
		delta.x *= 2.0;
		delta.y *= 2.0;
	}
	int	colision = 0;
	data->pos.x += delta.x;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] > 0.0)
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] > 0.0)
		colision = 1;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] > 0.0)
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] > 0.0)
		colision = 1;
	if (colision)
		data->pos.x -= delta.x;
	colision = 0;
	data->pos.y += delta.y;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] > 0.0)
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] > 0.0)
		colision = 1;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] > 0.0)
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] > 0.0)
		colision = 1;
	if (colision)
		data->pos.y -= delta.y;
	int32_t	x, y;
	mlx_get_mouse_pos(data->mlx, &x, &y);
	(void)y;
	data->dir = data->dir_delta + x * 0.001;
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
		if (next_cell.x >= 0 && next_cell.x < data->map_size.x && next_cell.y >= 0 && next_cell.y < data->map_size.y)
		{
			if (data->map[next_cell.x][next_cell.y] == 2)
			{
				data->map[next_cell.x][next_cell.y] = 3;
				data->door.cell = next_cell;
				data->door.moved = 0.0;
				data->door.opens = true;
			}
			else if (data->map[next_cell.x][next_cell.y] == -1)
			{
				data->map[next_cell.x][next_cell.y] = 3;
				data->door.cell = next_cell;
				data->door.moved = 1.0;
				data->door.opens = false;
			}
			else if (data->map[next_cell.x][next_cell.y] == 3)
				data->door.opens = !data->door.opens;
		}
	}
}

int	main(void)
{
	t_data	data;
	mlx_t	*mlx;

	data.map_size.x = 10;
	data.map_size.y = 15;
	data.map = malloc(sizeof(*(data.map)) * data.map_size.x);
	data.map[0] = (int []){1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	data.map[1] = (int []){1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[2] = (int []){1, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[3] = (int []){1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1};
	data.map[4] = (int []){1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[5] = (int []){1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[6] = (int []){1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
	data.map[7] = (int []){1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[8] = (int []){1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
	data.map[9] = (int []){1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1};
	data.texture[0] = mlx_load_png("./assets/rdr2.png");
	data.texture[1] = mlx_load_png("./assets/rdr2.png");
	data.texture[2] = mlx_load_png("./assets/rdr2.png");
	data.texture[3] = mlx_load_png("./assets/rdr2.png");
	data.texture[4] = mlx_load_png("./assets/rdr22.png");
	data.texture[5] = mlx_load_png("./assets/rdr22.png");
	data.fov = FOV * PI / 180.0;
	data.pos = (t_vec2f){.x = 2.0f, .y = 2.0f};
	npc = (t_vec2f){.x = 3.0f, .y = 3.0f};
	data.door.moved = -1.0;
	mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	mlx_set_window_limit(mlx, 160, 90, __INT_MAX__, __INT_MAX__);
	data.win = mlx_new_image(mlx, mlx->width, mlx->height);
	data.ray_lenghts = malloc(sizeof(*(data.ray_lenghts)) * data.win->width);
	mlx_set_cursor_mode(mlx, MLX_MOUSE_DISABLED);
	mlx_focus(mlx);
	data.dir_delta = 0.0 * PI / 180.0;
	data.win_wh = data.win->width / 2;
	data.dis = (float)data.win_wh / tanf(data.fov / 2.0);
	data.mlx = mlx;
	mlx_image_to_window(mlx, data.win, 0, 0);
	mlx_loop_hook(mlx, ft_hook, &data);
	mlx_scroll_hook(mlx, scroll, &data);
	mlx_key_hook(mlx, ft_keyhook, &data);
	mlx_resize_hook(mlx, ft_resize_hook, &data);
	mlx_loop(mlx);
	mlx_terminate(mlx);
}
