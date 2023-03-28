#include "../include/cub3d.h"

t_vec2f	npc;
float	*lens;

void ft_hook(void* param)
{
	t_data *const	data = param;
	int32_t		i;
	int				j;
	int				k;
	float			a;
	float			del_a;
	t_vec2f			dir;
	t_vec2f			unit;
	t_vec2i			step;
	t_vec2f			delta;
	t_vec2i			pos;
	t_vec2f			leng;
	float			len;
	int				hoz;

	ft_memset(data->win->pixels, 0, data->win->width * data->win->height * 4);
	i = 0;
	while (i < (int)data->win->width)
	{
		del_a = atanf((float)(i - (int)data->win_wh) / data->dis);
		a = data->dir + del_a;
		float cos_a = cosf(a);
		float sin_a = sinf(a);

		pos = (t_vec2i){.x = floorf(data->pos.x), .y = floorf(data->pos.y)};
		dir = (t_vec2f){.x = -sin_a, .y = cos_a};
		if (dir.x >= 0.0)
		{
			step.x = 1;
			delta.x = 1.0 - (data->pos.x - pos.x);
		}
		else
		{
			step.x = -1;
			delta.x = (data->pos.x - pos.x);
		}
		if (dir.y >= 0.0)
		{
			step.y = 1;
			delta.y = 1.0 - (data->pos.y - pos.y);
		}
		else
		{
			step.y = -1;
			delta.y = (data->pos.y - pos.y);
		}
		if (sin_a != 0.0)
		{
			unit.x = fabsf(1.0f / sin_a);
			leng.x = unit.x * delta.x;
		}
		if (cos_a != 0.0)
		{
			unit.y = fabsf(1.0f / cos_a);
			leng.y = unit.y * delta.y;
		}
		while (1)
		{
			if (sin_a == 0.0)
			{
				len = leng.y;
				pos.y += step.y;
				leng.y += unit.y;
				hoz = 0;
			}
			else if (cos_a == 0.0)
			{
				len = leng.x;
				pos.x += step.x;
				leng.x += unit.x;
				hoz = 1;
			}
			else if (leng.x < leng.y)
			{
				len = leng.x;
				pos.x += step.x;
				leng.x += unit.x;
				hoz = 1;
			}
			else
			{
				len = leng.y;
				pos.y += step.y;
				leng.y += unit.y;
				hoz = 0;
			}
			if (len > 50.0)
				break ;
			if (len > 0.05 && pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y)
			{
				if (data->map[pos.x][pos.y] == 1)
				{
					len *= cosf(del_a);
					len = data->dis / len;
					lens[i] = len;
					j = ((int)data->win->height - len) / 2;
					k = 0;
					while (k++ < len)
					{
						if (j >= 0 && j < (int)data->win->height)
						{
							if (hoz && step.x == 1)
								mlx_put_pixel(data->win, i, j, 0xFFFFFFFF);
							else if (hoz)
								mlx_put_pixel(data->win, i, j, 0xBBBBBBFF);
							else if (step.y == 1)
								mlx_put_pixel(data->win, i, j, 0x999999FF);
							else
								mlx_put_pixel(data->win, i, j, 0xDDDDDDFF);
						}
						j++;
					}
					break ;
				}
				if (data->map[pos.x][pos.y] == 2)
				{
					if (hoz && (!cos_a || leng.x - unit.x / 2.0 < leng.y))
						len = leng.x - unit.x / 2.0;
					else if (!hoz && (!sin_a || leng.y - unit.y / 2.0 <= leng.x))
						len = leng.y - unit.y / 2.0;
					else
						continue ;
					len *= cosf(del_a);
					len = data->dis / len;
					lens[i] = len;
					j = ((int)data->win->height - len) / 2;
					k = 0;
					while (k++ < len)
					{
						if (j >= 0 && j < (int)data->win->height)
						{
							if (hoz && step.x == 1)
								mlx_put_pixel(data->win, i, j, 0x0000FFFF);
							else if (hoz)
								mlx_put_pixel(data->win, i, j, 0x0000BBFF);
							else if (step.y == 1)
								mlx_put_pixel(data->win, i, j, 0x000099FF);
							else
								mlx_put_pixel(data->win, i, j, 0x0000DDFF);
						}
						j++;
					}
					break ;
				}
			}
		}
		i++;
	}
	t_vec2f	npc_r;
	t_vec2f	npc_d;
	npc_d.x = npc.x - data->pos.x;
	npc_d.y = npc.y - data->pos.y;
	npc_d.x *= -1;
	npc_r.x = npc_d.x * cosf(data->dir) - npc_d.y * sinf(data->dir);
	npc_r.y = npc_d.x * sinf(data->dir) + npc_d.y * cosf(data->dir);
	int32_t npc_x;
	npc_x = npc_r.x * (data->dis / npc_r.y);
	npc_x += data->win_wh;
	npc_r.y = data->dis / npc_r.y;//npc_r.y > 0.05
	i = -npc_r.y / 2.0;
	while (i < (npc_r.y) / 2.0)
	{
		if (npc_x + i >= 0 && npc_x + i < (int)data->win->width)
		{
			j = ((int)data->win->height - npc_r.y) / 2;
			k = 0;
			while (k++ < npc_r.y)
			{
				if (j >= 0 && j < (int)data->win->height && npc_r.y > lens[npc_x + i])
					mlx_put_pixel(data->win, npc_x + i, j, 0xFF0000FF);
				j++;
			}
		}
		i++;
	}

	t_vec2f	move = {.x = 0.0, .y = 0.0};

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
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y])
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y])
		colision = 1;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y])
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y])
		colision = 1;
	if (colision)
		data->pos.x -= delta.x;
	colision = 0;
	data->pos.y += delta.y;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y])
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y])
		colision = 1;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y])
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y])
		colision = 1;
	if (colision)
		data->pos.y -= delta.y;
	int32_t	x, y;
	mlx_get_mouse_pos(data->mlx, &x, &y);
	(void)y;
	data->dir = data->dir_delta + x * 0.001;
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
}

void	ft_keyhook(mlx_key_data_t keydata, void *param)
{
	t_data *const	data = param;
	static int		mode;

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
	data.fov = FOV * PI / 180.0;
	data.pos = (t_vec2f){.x = 2.0f, .y = 2.0f};
	npc = (t_vec2f){.x = 3.0f, .y = 3.0f};
	mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	mlx_set_window_limit(mlx, 160, 90, __INT_MAX__, __INT_MAX__);
	data.win = mlx_new_image(mlx, mlx->width, mlx->height);
	lens = malloc(sizeof(*lens) * data.win->width);
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
