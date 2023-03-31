#include "../include/cub3d.h"

t_vec2f	npc;
bool	opening;
float	door;

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

int32_t factor_pixel(int c, float f)
{
	if (f > 1.0)
		return (0xFF000000);
    return (255 << 24 | (int)((c >> 16 & 0xFF) * f) << 16 \
			| (int)((c >> 8 & 0xFF) * f) << 8 
			| (int)((c & 0xFF) * f));
}

void txt_to_img(mlx_image_t *dst, mlx_texture_t *src, t_vec2i loc, float x_hit)
{
	int	y_temp;
	int	index;
	t_vec2f src_loc;
	int	draw_height;

	x_hit -= (int)x_hit;
	if (x_hit < 0)
		x_hit = 1 + x_hit;
	draw_height = dst->height - loc.y * 2;
	y_temp = loc.y;
	if (loc.y < 0)
		loc.y = 0;
	while (loc.y < y_temp + draw_height && loc.y < (int)dst->height)
	{
		src_loc.y = (float)src->height / draw_height * (loc.y - y_temp);
		src_loc.x = src->width * x_hit;
		index = (int)src_loc.y * src->width + src_loc.x;
		if (((int *)src->pixels)[index])
			((int *)dst->pixels)[loc.y * dst->width + loc.x] \
			= factor_pixel(((int *)src->pixels)[index], 1);
		loc.y++;
	}
}

void ft_hook(void* param)
{
	t_data *const	data = param;
	int32_t		i;
	int				j;
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

	//ft_memset(data->win->pixels, 0, data->win->width * data->win->height * 4);
	char *str = ft_itoa(1.0 / data->mlx->delta_time);
	mlx_delete_image(data->mlx, data->prev_text);
	data->prev_text = mlx_put_string(data->mlx, str, 3, 0);
	free(str);
	draw_rectangle(data->win, 0, 0, data->win->width, data->win->height / 2, 0xFF00FF00);
	draw_rectangle(data->win, 0, data->win->height / 2, data->win->width, data->win->height / 2, 0xFFFF0000);
	ft_memset(data->depth_buffer, 0, data->win->width * 4);
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
				float hit;
				if (data->map[pos.x][pos.y] == 1)
				{
					hit = len;
					len *= cosf(del_a);
					len = data->dis / len;
					data->depth_buffer[i] = len;
					j = ((int)data->win->height - len) / 2;
					if (hoz && step.x == 1)
						txt_to_img(data->win, data->texture[0], (t_vec2i){i, j}, -(data->pos.y + dir.y * hit));
					else if (hoz)
						txt_to_img(data->win, data->texture[1], (t_vec2i){i, j}, (data->pos.y + dir.y * hit));
					else if (step.y == 1)
						txt_to_img(data->win, data->texture[2], (t_vec2i){i, j}, (data->pos.x + dir.x * hit));
					else
						txt_to_img(data->win, data->texture[3], (t_vec2i){i, j}, -(data->pos.x + dir.x * hit));
					break ;
				}
				if (data->map[pos.x][pos.y] == 2)
				{
					float len2;
					if (hoz && (!cos_a || leng.x - unit.x / 2.0 < leng.y))
						len2 = leng.x - unit.x / 2.0;
					else if (!hoz && (!sin_a || leng.y - unit.y / 2.0 <= leng.x))
						len2 = leng.y - unit.y / 2.0;
					else
						continue ;
					hit = len2;
					float hit_n;
					if (hoz)
						hit_n = data->pos.y + dir.y * hit;
					else
						hit_n = data->pos.x + dir.x * hit;
					hit_n -= (int)hit_n;
					if (hit_n - (int)hit_n < door)
						continue ;
					len2 *= cosf(del_a);
					len2 = data->dis / len2;
					data->depth_buffer[i] = len2;
					j = ((int)data->win->height - len2) / 2;
					txt_to_img(data->win, data->texture[1], (t_vec2i){i, j}, hit_n - door);
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
	if (npc_r.y > 0.05)
	{
		int32_t npc_x;
		npc_x = npc_r.x * (data->dis / npc_r.y);
		npc_x += data->win_wh;
		float hit;
		npc_r.y = data->dis / npc_r.y;//npc_r.y > 0.05
		i = -npc_r.y / 2.0;
		while (i < (npc_r.y) / 2.0)
		{
			if (npc_x + i >= 0 && npc_x + i < (int)data->win->width && (npc_r.y > data->depth_buffer[npc_x + i] || data->depth_buffer[npc_x + i] == 0.0))
			{
				j = ((int)data->win->height - npc_r.y) / 2;
				hit = (i + npc_r.y / 2) / npc_r.y;
				txt_to_img(data->win, data->texture[0], (t_vec2i){npc_x + i, j}, hit);
				// k = 0;
				// while (k++ < npc_r.y)
				// {
				// 	if (j >= 0 && j < (int)data->win->height && npc_r.y > data->depth_buffer[npc_x + i])
				// 		mlx_put_pixel(data->win, npc_x + i, j, 0xFF0000FF);
				// 	j++;
				// }
			}
			i++;
		}
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
	if (mlx_is_key_down(data->mlx, MLX_KEY_Q))
		data->dir += PI / 180;
	if (mlx_is_key_down(data->mlx, MLX_KEY_E))
		data->dir -= PI / 180;
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
	if (opening && door < 1.0)
	{
		door += data->mlx->delta_time;
		if (door > 1.0)
			door = 1.0;
	}
	if (!opening && door > 0.0)
	{
		door -= data->mlx->delta_time;
		if (door < 0.0)
			door = 0.0;
	}
	static double z;
	z += data->mlx->delta_time;
	if (z > 1.0)
	{
		printf("%f\n", z);
		z -= 1.0;
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
	free(data->depth_buffer);
	data->depth_buffer = malloc(sizeof(*data->depth_buffer) * data->win->width);
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
	if (keydata.key == MLX_KEY_F && keydata.action == MLX_RELEASE)
		opening = !opening;
}

const char	*types[TEXTURE_CNT] = {"NO", "SO", "WE", "EA"};

t_input_types	get_input_type(t_data *data, char *line)
{
	(void)data;
	int i;

	if (line == NULL)
		return (INVALID);
	if (ft_strlen(line) == 1 && *line == '\n')
		return (NEWLINE);
	i = -1;
	while (types[++i] != NULL)
		if (!ft_strncmp(types[i], line, 2))
			return (TEXTURE);
	if (!ft_strncmp("F", line, 1) || !ft_strncmp("C", line, 1))
		return (COLOR);
	return (INVALID);
}

int	check_int(char *str)
{
	size_t	i;

	if ((ft_strlen(str) > 10) || *str == '\0')
		return (1);
	i = -1;
	while (++i < ft_strlen(str))
		if (str[i] < '0' || str[i] > '9' \
			|| (str[i] > "2147483647"[i] && ft_strlen(str) > 9))
			return (1);
	i = ft_atoi(str);
	return (i < 256);
}

int	count_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' && str[i] != '\n')
		i++;
	return (i);
}

int load_map(t_data *data, t_input *in)
{
	(void)data;
	(void)in;
	return (0);
}

int	load_colors(t_data *d, t_input *in)
{
	while (in->i != 0 && get_input_type(d, in->input[in->i]) == NEWLINE)
		in->i++;
	if (get_input_type(d, in->input[in->i]) == COLOR
		&& get_input_type(d, in->input[in->i + 1]) == COLOR)
	{
		
	}
	else
		return (printf("Error\nInvalid or missing colors!\n"), 1);
	return (0);
}

int	load_textures(t_data *d, t_input *in)
{
	int	i;

	while (in->i != 0 && get_input_type(d, in->input[in->i]) == NEWLINE)
		in->i++;
	while (get_input_type(d, in->input[in->i]) == TEXTURE)
	{
		i = 0;
		while (i < TEXTURE_CNT && ft_strncmp(types[i], in->input[in->i], 2))
			i++;
		if (i < TEXTURE_CNT && !ft_strncmp(types[i], in->input[in->i], 2))
		{
			in->input[in->i][ft_strlen(in->input[in->i]) - 1] = 0;
			in->input[in->i][ft_strlen(in->input[in->i]) - 1] = 0; //Only needed for windows I think
			if (d->texture[i] != NULL)
				return (printf("Error\nTexture duplicate in input file!\n"), 1);
			d->texture[i] = mlx_load_png(in->input[in->i] + \
				count_spaces(in->input[in->i] + 2) + 2);
			if (d->texture[i] == NULL)
				return (printf("Error\nTexture load fail!\n"), 1);
		}
		in->i++;
	}
	i = -1;
	while (++i < TEXTURE_CNT)
		if (d->texture[i] == NULL)
			return (printf("Error\nMissing textures!\n"), 1);
	return (0);
}

int	read_input(t_input *in_data)
{
	char	**buff;
	char	*ret;
	int		i;
	int		i2;

	i = 0;
	ret = (void *)1;
	while (ret != NULL)
	{
		buff = malloc((i + 1) * sizeof(char *));
		if (buff == NULL)
			return (1);
		i2 = -1;
		while (++i2 < i - 1)
			buff[i2] = in_data->input[i2];
		if (i++ > 0)
			buff[i2++] = ret;
		buff[i2] = NULL;
		if (in_data->input != NULL)
			free(in_data->input);
		in_data->input = buff;
		ret = get_next_line(in_data->fd);
	}
	return (0);
}

int	load_data(t_data *data, t_input *in)
{
	if (ft_strlen(in->filename) < 5 \
		|| ft_strcmp(in->filename + ft_strlen(in->filename) - 4, ".cub"))
		return (printf("Error\nInput filename not valid!\n"), 1);
	in->fd = open(in->filename, O_RDONLY);
	if (in->fd == -1)
		return (printf("Error\nFailed to open input file!\n"), 1);
	if (read_input(in) || in->input == NULL)
		return (printf("Error\nError whilst reading input file!\n"), 1);
	if (get_input_type(data, in->input[in->i]) == TEXTURE)
	{
		if (load_textures(data, in) || load_colors(data, in))
			return (1);
	}
	else if (load_colors(data, in) || load_textures(data, in))
		return (1);
	if (load_map(data, in))
		return (1);
	return (0);
}

int	init_data(t_data *data, t_input *in_data)
{
	int	i;

	i = 0;
	while (i < TEXTURE_CNT)
		data->texture[i++] = NULL;
	data->fov = FOV * PI / 180.0;
	data->pos = (t_vec2f){.x = 2.0f, .y = 2.0f};
	npc = (t_vec2f){.x = 3.0f, .y = 3.0f};
	opening = false;
	door = 0.0;
	data->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (data->mlx == NULL)
		return (printf("Error\nFailed to initialize MLX instance!\n"), 1);
	mlx_set_window_limit(data->mlx, 160, 90, __INT_MAX__, __INT_MAX__);
	data->win = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	if (data->win == NULL)
		return (printf("Error\nFailed to create MLX image!\n"), 1);
	data->dir_delta = 0.0 * PI / 180.0;
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	data->depth_buffer = NULL;
	data->depth_buffer = malloc(sizeof(float *) * data->win->width);
	return (load_data(data, in_data));
}

void destroy_data(t_data *data, t_input *in)
{
	int	i;

	if (data->mlx)
	{
		// i = -1;
		// while (++i < TEXTURE_CNT)
		// 	if (data->texture[i] != NULL)
		// 		mlx_delete_texture(data->texture[i]);
		mlx_terminate(data->mlx);
	}
	if (data->depth_buffer != NULL)
		free(data->depth_buffer);
	if (in->fd != -1)
		close(in->fd);
	i = 0;
	while (in->input != NULL && in->input[i])
		free(in->input[i++]);
	free(in->input);
}

int	main(int argc, char **argv)
{
	t_input	in;
	t_data	data;

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
	if (argc != 2)
		return (printf("Error\nWrong number of arguments!\n"), 1);
	in.filename = argv[1];
	in.fd = -1;
	in.input = NULL;
	in.i = 0;
	if (!init_data(&data, &in))
	{
		mlx_set_cursor_mode(data.mlx, MLX_MOUSE_DISABLED);
		mlx_focus(data.mlx);
		if (mlx_image_to_window(data.mlx, data.win, 0, 0) != -1)
		{
			mlx_loop_hook(data.mlx, ft_hook, &data);
			mlx_scroll_hook(data.mlx, scroll, &data);
			mlx_key_hook(data.mlx, ft_keyhook, &data);
			mlx_resize_hook(data.mlx, ft_resize_hook, &data);
			mlx_loop(data.mlx);
		}
		printf("Success!\n");
	}
	destroy_data(&data, &in);
	return (printf("Exited!\n"), 0);
}
