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
		dir = (t_vec2f){.x = -sinf(a), .y = cosf(a)};
		float cos = cosf(a);
		float sin = sinf(a);

		if (!cos || !sin)
		{
			i++;
			continue ;
		}
		pos = (t_vec2i){.x = floorf(data->pos.x), .y = floorf(data->pos.y)};
		if (dir.x >= 0.0)
		{
			step.x = 1;
			delta.y = 1.0 - (data->pos.x - pos.x);
		}
		else
		{
			step.x = -1;
			delta.y = (data->pos.x - pos.x);
		}
		if (dir.y >= 0.0)
		{
			step.y = 1;
			delta.x = 1.0 - (data->pos.y - pos.y);
		}
		else
		{
			step.y = -1;
			delta.x = (data->pos.y - pos.y);
		}
		unit.y = fabsf(1.0f / sinf(a));
		leng.y = unit.y * delta.y;
		unit.x = fabsf(1.0f / cosf(a));
		leng.x = unit.x * delta.x;
		// printf("new ray delta.y: %f, delta.x: %f\n", delta.y, delta.x);
		while (1)
		{
			// printf("i: %d, unit.y: %f, unit.x: %f, leng.y: %f, leng.x %f\n", i, unit.y, unit.x, leng.y, leng.x);
			if (leng.y < leng.x)
			{
				len = leng.y;
				pos.x += step.x;
				leng.y += unit.y;
				hoz = 1;
			}
			else
			{
				len = leng.x;
				pos.y += step.y;
				leng.x += unit.x;
				hoz = 0;
			}
			if (len > 50.0)
				break ;
			// printf("len: %f\n", len);
			if (len > 0.05 && pos.x >= 0 && pos.x < 6 && pos.y >= 0 && pos.y < 5 && data->map[pos.x][pos.y])
			{
				if (cosf(del_a) == 0.0 || len == 0.0)
					printf("cosf: %f, len: %f\n", cosf(del_a), len);
				len *= cosf(del_a);
				// if (len < 0.01)
				// 	len = 0.01;
				// printf("draw len: %f\n", len);
				len = data->dis / len;
				// printf("after len: %f\n", len);
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
		move.y += 0.01;
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
		move.y -= 0.01;
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		move.x += 0.01;
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		move.x -= 0.01;

	data->pos.x += move.x * cosf(data->dir) - move.y * sinf(data->dir);
	data->pos.y += move.x * sinf(data->dir) + move.y * cosf(data->dir);
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		data->dir += 0.01;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		data->dir -= 0.01;
}

void	scroll(double xdelta, double ydelta, void *param)
{
	t_data *const	data = param;

	(void)(xdelta);
	data->fov += ydelta * 0.01;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
}

int	main(void)
{
	t_data	data;
	mlx_t	*mlx;

	data.map = malloc(sizeof(*(data.map)) * 6);
	data.map[0] = (int []){1, 1, 1, 1, 1};
	data.map[1] = (int []){1, 0, 0, 0, 1};
	data.map[2] = (int []){1, 0, 0, 0, 1};
	data.map[3] = (int []){1, 0, 0, 0, 1};
	data.map[4] = (int []){1, 0, 0, 0, 1};
	data.map[5] = (int []){1, 1, 1, 1, 1};
	data.fov = FOV * PI / 180.0;
	data.pos = (t_vec2f){.x = 2.0f, .y = 2.0f};
	data.dir = 0.0f;
	mlx = mlx_init(WIDTH, HEIGHT, "cub3D", 0);
	data.win_wh = mlx->width / 2;
	data.dis = (float)data.win_wh / tanf(data.fov / 2.0);
	data.mlx = mlx;
	data.win = mlx_new_image(mlx, mlx->width, mlx->height);
	mlx_image_to_window(mlx, data.win, 0, 0);
	mlx_loop_hook(mlx, ft_hook, &data);
	mlx_scroll_hook(mlx, scroll, &data);
	mlx_loop(mlx);
	mlx_terminate(mlx);
}
