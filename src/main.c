#include "../include/cub3d.h"

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
	while (i < data->mlx->width)
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
			unit.x = fabsf(1.0f / sin_a);
		leng.x = unit.x * delta.x;
		if (cos_a != 0.0)
			unit.y = fabsf(1.0f / cos_a);
		leng.y = unit.y * delta.y;
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
			if (len > 0.05 && pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y])
			{
				len *= cosf(del_a);
				len = data->dis / len;
				j = (data->mlx->height - len) / 2;
				k = 0;
				while (k++ < len)
				{
					if (j >= 0 && j < data->mlx->height)
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
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] == 1)
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] == 1)
		colision = 1;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] == 1)
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] == 1)
		colision = 1;
	if (colision)
		data->pos.x -= delta.x;
	colision = 0;
	data->pos.y += delta.y;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] == 1)
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y + 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] == 1)
		colision = 1;
	pos.x = floorf(data->pos.x + 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] == 1)
		colision = 1;
	pos.x = floorf(data->pos.x - 0.25);
	pos.y = floorf(data->pos.y - 0.25);
	if (pos.x >= 0 && pos.x < data->map_size.x && pos.y >= 0 && pos.y < data->map_size.y && data->map[pos.x][pos.y] == 1)
		colision = 1;
	if (colision)
		data->pos.y -= delta.y;
	int32_t	x, y;
	mlx_get_mouse_pos(data->mlx, &x, &y);
	(void)y;
	data->dir = x * 0.01;
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

int	main(void)
{
	t_data	data;
	mlx_t	*mlx;

	data.map_size.x = 10;
	data.map_size.y = 15;
	data.map = malloc(sizeof(*(data.map)) * data.map_size.x);
	data.map[0] = (int []){1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	data.map[1] = (int []){1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[2] = (int []){1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[3] = (int []){1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1};
	data.map[4] = (int []){1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[5] = (int []){1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[6] = (int []){1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};
	data.map[7] = (int []){1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
	data.map[8] = (int []){1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1};
	data.map[9] = (int []){1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1};
	data.fov = FOV * PI / 180.0;
	data.pos = (t_vec2f){.x = -2.0f, .y = -2.0f};
	data.dir = 0.0 * PI / 180.0;
	mlx = mlx_init(WIDTH, HEIGHT, "cub3D", 0);
	data.win_wh = mlx->width / 2;
	data.dis = (float)data.win_wh / tanf(data.fov / 2.0);
	data.mlx = mlx;
	data.win = mlx_new_image(mlx, mlx->width, mlx->height);
	mlx_set_cursor_mode(mlx, MLX_MOUSE_DISABLED);
	mlx_image_to_window(mlx, data.win, 0, 0);
	mlx_loop_hook(mlx, ft_hook, &data);
	mlx_scroll_hook(mlx, scroll, &data);
	mlx_loop(mlx);
	mlx_terminate(mlx);
}
