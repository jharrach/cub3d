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

	ft_memset(data->win->pixels, 0, data->win->width * data->win->height * 4);
	i = 0;
	while (i < data->mlx->width)
	{
		del_a = atanf((float)(i - (int)data->win_wh) / data->dis);
		a = data->dir + del_a;
		dir = (t_vec2f){.x = -sinf(a), .y = cosf(a)};
		if (dir.x >= 0.0)
		{
			step.x = 1;
			delta.y = 1.0 - (data->pos.x - (int)data->pos.x);
		}
		else
		{
			step.x = -1;
			delta.y = (data->pos.x - (int)data->pos.x);
		}
		if (dir.y >= 0.0)
		{
			step.y = 1;
			delta.x = 1.0 - (data->pos.y - (int)data->pos.y);
		}
		else
		{
			step.y = -1;
			delta.x = 1.0 - (data->pos.y - (int)data->pos.y);
		}
		if (a != 0.0)
		{
			unit.y = fabsf(1.0f / sinf(a));
			leng.y = unit.y * delta.y;
		}
		// else
		// {
		// 	unit.y = 50.0;
		// 	leng.y = unit.y * delta.y;
		// }
		if (a != 90.0)
		{
			unit.x = fabsf(1.0f / cosf(a));
			leng.x = unit.x * delta.x;
		}
		// else
		// {
		// 	unit.x = 50.0;
		// 	leng.x = unit.x * delta.x;
		// }
		pos = (t_vec2i){.x = data->pos.x, .y = data->pos.y};
		while (1)
		{
			if (a != 0 && leng.y < leng.x)
			{
				len = leng.y;
				pos.x += step.x;
				leng.y += unit.y;
			}
			else if (a != 90.0)
			{
				len = leng.x;
				pos.y += step.y;
				leng.x += unit.x;
			}
			if (len > 50.0)
				break ;
			if (pos.x >= 0 && pos.x < 6 && pos.y >= 0 && pos.y < 5 && data->map[pos.x][pos.y])
			{
				len = data->dis / (len * cosf(del_a));
				j = data->mlx->height / 2.0 - len / 2.0;
				k = 0;
				while (k++ < len)
				{
					if (j >= 0 && j < data->mlx->height)
						mlx_put_pixel(data->win, i, j, 0xFFFFFFFF);
					j++;
				}
				break ;
			}
		}
		i++;
	}

	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
		data->pos.y += 0.01;
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
		data->pos.y -= 0.01;
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
		data->pos.x += 0.01;
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
		data->pos.x -= 0.01;

	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		data->dir += 0.01;
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		data->dir -= 0.01;
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
	mlx_loop(mlx);
	mlx_terminate(mlx);
}
