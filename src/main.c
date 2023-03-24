#include "../include/cub3d.h"

int32_t ft_pixel(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (a << 24 | b << 16 | g << 8 | r);
}

void print_vector(t_p v)
{
	printf("X:%f, Y:%f\n", v.x, v.y);
}

void draw_rectangle(mlx_image_t *img, int x, int y, int w, int h, int col)
{
	int x_temp;

	h += y--;
	x_temp = x;
	while (++y < h)
	{
		while (x < x_temp + w)
			((int *)img->pixels)[y * img->width + x++] = col;
		x = x_temp;
	}
}

void draw_line_from_image(mlx_image_t *dest, mlx_texture_t *src, int x_dest, int y_dest, int h, float temp_wall_hit_var)
{
	int y_temp;
	int x_src;
	int y_src;

	y_temp = y_dest--;
	if (y_dest < -1)
		y_dest = -1;
	while (++y_dest < y_temp + h)
	{
		y_src = (float)src->height / h * (y_dest - y_temp);
		x_src = src->width * temp_wall_hit_var;
		((int *)dest->pixels)[y_dest * dest->width + x_dest] = ((int *)src->pixels)[y_src * src->width + x_src];
		if (y_dest >= (int)dest->height - 1)
			break ;
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
	data->player_location.y = 3;
	data->forward.y = -0.1;
	data->forward.x = 0;
	data->time = 0;
	data->prev_text = NULL;
	data->test = mlx_load_png("./wb.png");
}

void find_collision_bad(t_data *data, t_p *point, float angle)
{
	t_p forward;

	forward.x = 0;
	forward.y = 0;
	while (true)
	{
		*point = add_vector(data->player_location, rotate_vector(forward, angle + data->player_direction));
		if ((*data->map)[(int)point->y][(int)point->x])
			break;
		forward.y -= 0.01;
	}
}

void cast_rays(t_data *data)
{
	float angle;
	t_p point;
	t_p draw;
	int draw_heigt;

	draw.x = -1;
	angle = -FOV / 2;
	while (++draw.x < WIDTH)
	{
		angle = -FOV / 2 + FOV * draw.x / WIDTH;
		find_collision_bad(data, &point, angle);
		draw_heigt = 1.5 * HEIGHT / (cos(angle * 3.141592653589793/180) * sqrt(pow(point.x-data->player_location.x, 2) + pow(point.y-data->player_location.y, 2)));
		draw.y = (HEIGHT - draw_heigt) / 2;
		//draw_rectangle(data->image, draw.x, draw.y, 1, draw_heigt, ft_pixel(255, 0, 0, 255));
		draw_line_from_image(data->image, data->test, draw.x, draw.y, draw_heigt, fabsf(point.x - (int)point.x));
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
	draw_rectangle(data->image, 0, 0, WIDTH, HEIGHT / 2, ft_pixel(0, 255, 0, 255));
	draw_rectangle(data->image, 0, HEIGHT / 2, WIDTH, HEIGHT / 2, ft_pixel(0, 0, 255, 255));
	cast_rays(data);
	char *str = ft_itoa(1 / (mlx_get_time() - data->time));
	mlx_delete_image(data->mlx, data->prev_text);
	data->prev_text = mlx_put_string(data->mlx, str, 3, 0);
	free(str);
	data->time = mlx_get_time();
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
	mlx_image_to_window(data.mlx, data.image, 0, 0);
	mlx_loop_hook(data.mlx, ft_hook, &data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
}
