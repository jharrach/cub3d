#include "../include/cub3d.h"

int32_t factor_pixel(int c, float f)
{
	if (f > 1.0)
		return (0xFF000000);
    return (255 << 24 | (int)((c >> 16 & 0xFF) * f) << 16 \
			| (int)((c >> 8 & 0xFF) * f) << 8 
			| (int)((c & 0xFF) * f));
}

void draw_rectangle(mlx_image_t *img, int x, int y, int w, int h, int col)
{
	int	x_temp;

	h += y--;
	x_temp = x;
	while (++y < h)
	{
		while (x < x_temp + w)
			((int *)img->pixels)[y * img->width + x++] = col;
		x = x_temp;
	}
}

void txt_to_img(mlx_image_t *dst, mlx_texture_t *src, t_vec2i loc, float x_hit)
{
	int	y_temp;
	int	index;
	t_vec2f src_loc;
	int	draw_height;

	draw_height = dst->height - loc.y * 2;
	y_temp = loc.y;
	if (loc.y < 0)
		loc.y = 0;
	while (loc.y < y_temp + draw_height && loc.y < (int)dst->height)
	{
		src_loc.y = (float)src->height / draw_height * (loc.y - y_temp);
		src_loc.x = src->width * x_hit;
		index = (int)src_loc.y * src->width + src_loc.x;
		((int *)dst->pixels)[loc.y++ * dst->width + loc.x] \
			= factor_pixel(((int *)src->pixels)[index], 1);
	}
}

t_vec2f rotate_vector(t_vec2f v, float radiant)
{
	t_vec2f v_res;

	v_res.x = cos(radiant) * v.x - sin(radiant) * v.y;
	v_res.y = sin(radiant) * v.x + cos(radiant) * v.y;
	return (v_res);
}

t_vec2f get_vector(float radiant, float length)
{
	return ((t_vec2f){cos(radiant) * length, sin(radiant) * length});
}

t_vec2f add_vector(t_vec2f v1, t_vec2f v2)
{
	t_vec2f v_res;

	v_res.x = v1.x + v2.x;
	v_res.y = v1.y + v2.y;
	return (v_res);
}

void init_data(t_data *data)
{
	data->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", 0);
	data->image = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	data->player_direction = 0.0f;
	data->player_location.x = 2.5;
	data->player_location.y = 1.5;
	data->speed = 0.02f;
	data->time = 0;
	data->prev_text = NULL;
	data->north = mlx_load_png("./highres.png");
	data->south = mlx_load_png("./rdr22.png");
}

void find_collision_bad(t_data *data, t_vec2f *point, float radiant)
{
	t_vec2f forward;

	forward = get_vector(radiant, 0.01);
	*point = data->player_location;
	while (true)
	{
		*point = add_vector(*point, forward);
		if ((*data->map)[(int)point->y][(int)point->x])
			break;
	}
}

void find_collision_good(t_data *data, t_vec2f *point, float radiant)
{
	//t_vec2i step;
	t_vec2f forward;

	forward.x = cos(radiant);
	forward.y = sin(radiant);

	*point = data->player_location;
	// step.x = (data->player_direction + angle >= 0 && data->player_direction < 180) * 2 - 1;
	// step.y = !(data->player_direction + angle >= 90 && data->player_direction < 270) * 2 - 1;
	while (true)
	{
		*point = add_vector(*point, forward);
		if ((*data->map)[(int)point->y][(int)point->x])
			break;
	}
}

void cast_rays(t_data *data)
{
	float angle;
	t_vec2f point;
	t_vec2i draw;
	int draw_height;

	draw.x = -1;
	while (++draw.x < WIDTH)
	{
		angle = -FOV / 2 + (FOV * draw.x) / WIDTH;
		find_collision_bad(data, &point, angle + data->player_direction);
		draw_height = HEIGHT / (cos(angle) * sqrt(pow(point.x-data->player_location.x, 2) + pow(point.y-data->player_location.y, 2)));
		draw.y = (HEIGHT - draw_height) / 2;
		if (point.y < data->player_location.y)
			txt_to_img(data->image, data->north, draw, fabsf(point.x - (int)point.x));
		else
			txt_to_img(data->image, data->south, draw, fabsf(point.x - (int)point.x));
	}
}

void ft_hook(void* param)
{
	t_data *data = param;
	t_vec2f temp_loc;
	
	if (mlx_is_key_down(data->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(data->mlx);
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT_SHIFT))
		data->speed = 0.08;
	else
		data->speed = 0.02;
	if (mlx_is_key_down(data->mlx, MLX_KEY_W))
	{
		temp_loc = add_vector(data->player_location, get_vector(data->player_direction, data->speed));
		if (!(*data->map)[(int)temp_loc.y][(int)temp_loc.x])
			data->player_location = temp_loc;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_A))
	{
		temp_loc = add_vector(data->player_location, get_vector(-90 * PI / 180 + data->player_direction, data->speed));
		if (!(*data->map)[(int)temp_loc.y][(int)temp_loc.x])
			data->player_location = temp_loc;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_S))
	{
		temp_loc = add_vector(data->player_location, get_vector(180 * PI / 180  + data->player_direction, data->speed));
		if (!(*data->map)[(int)temp_loc.y][(int)temp_loc.x])
			data->player_location = temp_loc;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_D))
	{
		temp_loc = add_vector(data->player_location, get_vector(90 * PI / 180  + data->player_direction, data->speed));
		if (!(*data->map)[(int)temp_loc.y][(int)temp_loc.x])
			data->player_location = temp_loc;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_LEFT))
	{
		data->player_direction -= 0.05;
		if (data->player_direction < 0)
			data->player_direction += 2 * PI;
	}
	if (mlx_is_key_down(data->mlx, MLX_KEY_RIGHT))
	{
		data->player_direction += 0.05;
		if (data->player_direction >= 2 * PI)
			data->player_direction -= 2 * PI;
	}
	draw_rectangle(data->image, 0, 0, WIDTH, HEIGHT / 2, 0xFF00FF00);
	draw_rectangle(data->image, 0, HEIGHT / 2, WIDTH, HEIGHT / 2, 0xFFFF0000);
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
