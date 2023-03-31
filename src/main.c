#include "../include/cub3d.h"

t_vec2f	npc;

void destroy_data(t_data *data, t_input *in, int ext, char *error)
{
	int	i;

	if (data != NULL && data->mlx)
	{
		// i = -1;
		// while (++i < TEXTURE_CNT)
		// 	if (data->texture[i] != NULL)
		// 		mlx_delete_texture(data->texture[i]);
		mlx_terminate(data->mlx);
	}
	if (data != NULL && data->ray_lenghts != NULL)
		free(data->ray_lenghts);
	if (in != NULL && in->fd != -1)
		close(in->fd);
	i = 0;
	while (in != NULL && in->input[i])
		free(in->input[i++]);
	free(in->input);
	if (error != NULL)
		printf("Error\n%s", error);
	if (ext == 1)
		exit(1);
}

void	ft_fps(t_data *data, int32_t x, int32_t y)
{
	static mlx_image_t	*img = NULL;
	char				*str;

	str = ft_itoa(1.0 / data->mlx->delta_time);
	if (!str)
		destroy_data(data, &data->in, 1, "ft_fps");
	mlx_delete_image(data->mlx, img);
	img = mlx_put_string(data->mlx, str, x, y);
	free(str);
	if (!img)
		destroy_data(data, &data->in, 1, "ft_fps");
}

void ft_hook(void* param)
{
	t_data *const	data = param;

	ft_fps(data, 0, 0);
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

const char	*types[TEXTURE_CNT] = {"NO", "SO", "WE", "EA"};

t_input_types	get_input_type(t_data *data, char *line)
{
	(void)data;
	int i;

	if (line == NULL)
		return (INVALID);
	if (ft_strlen(line) == 1 && *line == '\n')
		return (NEWLINE);
	else if (ft_strlen(line) == 2 && *line == '\r' && line[1] == '\n') //Only windows
		return (NEWLINE); //Only windows
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

int	cnt_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] == ' ' && str[i] != '\n')
		i++;
	return (i);
}

int load_map(t_data *d, t_input *in)
{
	while (*in->i != *in->input && get_input_type(d, *in->i) == NEWLINE)
		in->i++;
	return (0);
}

int	load_colors(t_data *d, t_input *in)
{
	while (*in->i != *in->input && get_input_type(d, *in->i) == NEWLINE)
		in->i++;
	if (get_input_type(d, *in->i) == COLOR
		&& get_input_type(d, *(in->i + 1)) == COLOR)
	{
		printf("DEBUG TEST: Colors validated\n");
		in->i += 2;
	}
	else
		return (printf("Error\nInvalid or missing colors!\n"), 1);
	return (0);
}

int	load_textures(t_data *d, t_input *in)
{
	int	i;

	while (*in->i != *in->input && get_input_type(d, *in->i) == NEWLINE)
		in->i++;
	while (get_input_type(d, *in->i) == TEXTURE)
	{
		i = 0;
		while (i < TEXTURE_CNT && ft_strncmp(types[i], *in->i, 2))
			i++;
		if (i < TEXTURE_CNT && !ft_strncmp(types[i], *in->i, 2))
		{
			(*in->i)[ft_strlen(*in->i) - 1] = 0;
			(*in->i)[ft_strlen(*in->i) - 1] = 0; //Only windows
			if (d->texture[i] != NULL)
				return (printf("Error\nTexture duplicate input file!\n"), 1);
			d->texture[i] = mlx_load_png(*in->i + cnt_spaces(*in->i + 2) + 2);
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

int	read_input(t_input *in)
{
	char	*ret;
	int		i;
	int		i2;

	i = 0;
	ret = (void *)1;
	while (ret != NULL)
	{
		in->i = malloc((i + 1) * sizeof(char *));
		if (in->i == NULL)
			return (1);
		i2 = -1;
		while (++i2 < i - 1)
			in->i[i2] = in->input[i2];
		if (i++ > 0)
			in->i[i2++] = ret;
		in->i[i2] = NULL;
		if (in->input != NULL)
			free(in->input);
		in->input = in->i;
		ret = get_next_line(in->fd);
	}
	return (0);
}

void	load_data(t_data *data, t_input *in)
{
	if (ft_strlen(in->filename) < 5 \
		|| ft_strcmp(in->filename + ft_strlen(in->filename) - 4, ".cub"))
		destroy_data(data, &data->in, 1, "Input filename not valid!\n");
	in->fd = open(in->filename, O_RDONLY);
	if (in->fd == -1)
		destroy_data(data, &data->in, 1, "Failed to open input file!\n");
	if (read_input(in) || in->i == NULL)
		destroy_data(data, &data->in, 1, "Error whilst reading input file!\n");
	if (get_input_type(data, *in->i) == TEXTURE)
	{
		if (load_textures(data, in) || load_colors(data, in))
			destroy_data(data, &data->in, 1, NULL);
	}
	else if (load_colors(data, in) || load_textures(data, in))
		destroy_data(data, &data->in, 1, NULL);
	load_map(data, in);
}

void	init_data(t_data *data)
{
	int	i;

	i = 0;
	while (i < TEXTURE_CNT)
		data->texture[i++] = NULL;
	data->fov = FOV * PI / 180.0;
	data->pos = (t_vec2f){.x = 2.0f, .y = 2.0f};
	npc = (t_vec2f){.x = 3.0f, .y = 3.0f};
	data->door.moved = -1.0;
	data->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (data->mlx == NULL)
		destroy_data(data, &data->in, 1, "Failed to init MLX instance!\n");
	mlx_set_window_limit(data->mlx, 160, 90, __INT_MAX__, __INT_MAX__);
	data->win = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	if (data->win == NULL)
		destroy_data(data, &data->in, 1, "Failed to create MLX image!\n");
	data->dir_delta = 0.0 * PI / 180.0;
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	data->ray_lenghts = NULL;
	data->ray_lenghts = malloc(sizeof(float *) * data->win->width);
	load_data(data, &data->in);
}

int	main(int argc, char **argv)
{
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
	data.in.filename = argv[1];
	data.in.fd = -1;
	data.in.input = NULL;
	init_data(&data);
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
	destroy_data(&data, &data.in, 0, NULL);
	return (printf("Exited!\n"), 0);
}
