#include "../include/cub3d.h"

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void ft_mlx_put_pixel(mlx_image_t* image, t_p v, uint32_t color)
{
	mlx_put_pixel(image, WIDTH / 2 + v.x * SCALE, HEIGHT / 2 + v.y * SCALE, color);
}

void print_vector(t_p v)
{
	printf("X:%f, Y:%f\n", v.x, v.y);
}

void draw_rectangle(mlx_image_t *img, t_p *v, int width, int height, int color)
{
	t_p v_temp;

	v_temp.x = v->x;
	v_temp.y = v->y;
	while (++v_temp.x < v->x + width)
	{
		v_temp.y  = v->y;
		while (++v_temp.y < v->y + height)
			mlx_put_pixel(img, v_temp.x, v_temp.y , color);
	}
}

t_p rotate_vector(t_p v, float angle)
{
	t_p v_res;

	angle *= (3.141592653589793/180);
	v_res.x = cos(angle) * v.x - sin(angle) * v.y;
	v_res.y = sin(angle) * v.x + cos(angle) * v.y;
	return (v_res);
}

t_p add_vector(t_p v1, t_p v2)
{
	t_p v_res;

	v_res.x = v1.x + v2.x;
	v_res.y = v1.y + v2.y;
	return (v_res);
}

t_p *scale_vector(t_p *v, float scale)
{
	v->x *= scale;
	v->y *= scale;
	return (v);
}

void init_data(t_data *data)
{
	data->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", 0);
	data->image = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->player_direction = 0.0f;
	data->player_location.x = 2.5;
	data->player_location.y = 1.5;
	data->forward.y = -0.1;
	data->forward.x = 0;
}

void draw_map(t_data *data)
{
	int x;
	int y;
	x = -1;
	while (++x <= 8)
	{
		y = -1;
		while (++y < 7 * SCALE)
			mlx_put_pixel(data->image, x * SCALE, y, ft_pixel(0, 0, 255, 255));
	}
	y = -1;
	while (++y <= 7)
	{
		x = -1;
		while (++x < 8 * SCALE)
			mlx_put_pixel(data->image, x, y * SCALE, ft_pixel(0, 0, 255, 255));
	}
	x = -1;
	while (++x < 8)
	{
		y = -1;
		while (++y < 7)
		{
			t_p point;
			point.x = x * SCALE;
			point.y = y * SCALE;
			draw_rectangle(data->image, &point, SCALE, SCALE, ft_pixel(!(*data->map)[y][x] * 255 , !(*data->map)[y][x] * 255, !(*data->map)[y][x] * 255, 255));
		}
	}
}

void clear_screen(t_data *data)
{
	t_p var;

	var.x = -1;
	while (++var.x < WIDTH)
	{
		var.y = -1;
		while (++var.y < HEIGHT)
			mlx_put_pixel(data->image, var.x, var.y, 0);
	}
}

void cast_rays(t_data *data)
{
	float angle;
	t_p point;
	t_p draw;
	int draw_heigt;

	draw.x = 8 * SCALE;
	angle = -FOV / 2;
	while (angle < FOV / 2)
	{
		while (true)
		{
			point = add_vector(data->player_location, rotate_vector(data->forward, angle + data->player_direction));
			if ((*data->map)[(int)point.y][(int)point.x])
				break;
			data->forward.y -= 0.01;
		}
		draw_heigt = HEIGHT / sqrt(pow(point.x-data->player_location.x, 2) + pow(point.y-data->player_location.y, 2));
		if (draw_heigt >= HEIGHT)
			draw_heigt = HEIGHT;
		draw.x++;
		draw.y = (HEIGHT - draw_heigt) / 2;
		draw_rectangle(data->image, &draw, 2, draw_heigt, ft_pixel(255, 0, 0, 255));
		draw_rectangle(data->image, scale_vector(&point, SCALE), 5, 5, ft_pixel(255, 0, 0, 255));
		angle += FOV / (WIDTH - 8 * SCALE) ;
		data->forward.y = -0.01;
	}
}

void ft_hook(void* param)
{
	t_data *data = param;
	t_p temp_loc;

	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		temp_loc = add_vector(data->player_location, rotate_vector(data->forward, data->player_direction));
		if (!(*data->map)[(int)temp_loc.y][(int)temp_loc.x])
			data->player_location = temp_loc;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		temp_loc = add_vector(data->player_location, rotate_vector(data->forward, -90 + data->player_direction));
		if (!(*data->map)[(int)temp_loc.y][(int)temp_loc.x])
			data->player_location = temp_loc;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		temp_loc = add_vector(data->player_location, rotate_vector(data->forward, 180 + data->player_direction));
		if (!(*data->map)[(int)temp_loc.y][(int)temp_loc.x])
			data->player_location = temp_loc;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		temp_loc = add_vector(data->player_location, rotate_vector(data->forward, 90 + data->player_direction));
		if (!(*data->map)[(int)temp_loc.y][(int)temp_loc.x])
			data->player_location = temp_loc;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
		data->player_direction -= 5;
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
		data->player_direction += 5;
	clear_screen(data);
	draw_map(data);
	mlx_put_pixel(data->image, data->player_location.x * SCALE, data->player_location.y * SCALE, ft_pixel(255, 0, 0, 255));
	cast_rays(data);
	mlx_image_to_window(data->mlx, data->image, 0, 0);
}

int	main(void)
{
	int map[7][8] = { {1, 1, 1, 1, 1, 1, 1, 1}, 
					  {1, 0, 0, 0, 0, 0, 0, 1}, 
					  {1, 0, 0, 0, 1, 1, 0, 1}, 
					  {1, 0, 0, 0, 0, 0, 0, 1}, 
					  {1, 1, 1, 1, 0, 0, 0, 1}, 
					  {1, 0, 0, 0, 0, 0, 0, 1}, 
					  {1, 1, 1, 1 ,1 ,1, 1, 1} };
	t_data data;
	data.map = &map;
	init_data(&data);
	mlx_loop_hook(data.mlx, ft_hook, &data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
}
