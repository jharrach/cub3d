/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/30 17:02:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/02 21:16:52 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void destroy_data(t_data *data, t_input *in, int ext, char *error)
{
	int	i;

	mlx_terminate(data->mlx);
	free(data->ray_lenghts);
	if (in->fd != -1)
		close(in->fd);
	i = 0;
	while (in->input != NULL && in->input[i] != NULL)
		free(in->input[i++]);
	free(in->input);
	if (error != NULL)
		printf("Error\n%s\n", error);
	if (ext == 1)
		exit(1);
}

const char*	get_type(int i)
{
  const char	*types[TEXTURE_CNT] = {"NO", "SO", "WE", "EA", "DO", "NP"};

  if (i < 0 || i > TEXTURE_CNT - 1)
	return (NULL);
  return (types[i]);
}

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
	while (++i < TEXTURE_CNT)
		if (!ft_strncmp(get_type(i), line, 2))
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

void	load_map(t_data *d, t_input *in)
{
	while (*in->i != *in->input && get_input_type(d, *in->i) == NEWLINE)
		in->i++;
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
		while (i < TEXTURE_CNT && ft_strncmp(get_type(i), *in->i, 2))
			i++;
		if (i < TEXTURE_CNT && !ft_strncmp(get_type(i), *in->i, 2))
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
	if (read_input(in) || in->i == NULL)
		destroy_data(data, in, 1, "Error whilst reading input file!");
	if (get_input_type(data, *in->i) == TEXTURE)
	{
		if (load_textures(data, in) || load_colors(data, in))
			destroy_data(data, in, 1, NULL);
	}
	else if (load_colors(data, in) || load_textures(data, in))
		destroy_data(data, in, 1, NULL);
	load_map(data, in);
}

void	init_data(t_data *data, char *fn)
{
	data->ray_lenghts = NULL;
	data->ray_angle = NULL;
	ft_memset(data->texture, 0, sizeof(data->texture));
	data->fov = FOV * PI / 180.0;
	data->pos = (t_vec2f){.x = 2.0f, .y = 4.0f};
	data->door.moved = -1.0;
	data->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	if (data->mlx == NULL)
		destroy_data(data, &data->in, 1, "Failed to init MLX window!");
	if (ft_strlen(fn) < 5 || ft_strcmp(fn + ft_strlen(fn) - 4, ".cub"))
		destroy_data(data, &data->in, 1, "Invalid filename type!");
	data->in.fd = open(fn, O_RDONLY);
	if (data->in.fd == -1)
		destroy_data(data, &data->in, 1, "Failed to open input file!");
	mlx_set_window_limit(data->mlx, 320, 180, __INT_MAX__, __INT_MAX__);
	data->win = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	if (data->win == NULL)
		destroy_data(data, &data->in, 1, "Failed to create MLX image!");
	data->win_entities = mlx_new_image(data->mlx, data->mlx->width, data->mlx->height);
	if (data->win_entities == NULL)
		destroy_data(data, &data->in, 1, "Failed to create MLX image!");
	data->mm_win = mlx_new_image(data->mlx, 200, 200);
	data->mm_win_h.x = data->mm_win->width / 2;
	data->mm_win_h.y = data->mm_win->height / 2;
	mlx_texture_t	*txt = mlx_load_png("textures/minimap.png");
	if (!txt)
		exit(42);
	data->mm_txt = mlx_texture_to_image(data->mlx, txt);
	if (data->mm_txt == NULL)
		destroy_data(data, &data->in, 1, "Failed to create MLX image!");
	data->ray_lenghts = malloc(sizeof(*(data->ray_lenghts)) * data->win->width);
	if (data->ray_lenghts == NULL)
		destroy_data(data, &data->in, 1, "Failed to allocate depth buffer!");
	data->dir_delta = 0.0 * PI / 180.0;
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	data->ray_angle = malloc(sizeof(*(data->ray_angle)) * data->win->width);//error
	update_ray_angles(data);
	load_data(data, &data->in);
	data->num_entities = 2;
	data->entity = malloc(sizeof(*(data->entity)) * data->num_entities);
	data->entity[0].pos = (t_vec2f){.x = -2.0, .y = -2.0};
	data->entity[0].enabled = true;
	data->entity[0].img = data->texture[5];
	data->entity[0].half_width = 0.5;
	data->entity[1].pos = (t_vec2f){.x = -2.0, .y = -4.0};
	data->entity[1].enabled = true;
	data->entity[1].img = data->texture[5];
	data->entity[1].half_width = 0.1;
}

int	main(int argc, char **argv)
{
	t_data	data;

	data.map_size.x = 15;
	data.map_size.y = 15;
	data.map = malloc(sizeof(*(data.map)) * data.map_size.x);
	data.map[0]  = (int []){1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	data.map[1]  = (int []){0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0};
	data.map[2]  = (int []){0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	data.map[3]  = (int []){0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0};
	data.map[4]  = (int []){0, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 0, 0, 0, 0};
	data.map[5]  = (int []){0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0};
	data.map[6]  = (int []){0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0};
	data.map[7]  = (int []){0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0};
	data.map[8]  = (int []){0, 0, 0, 1, 1, 1, 1, 2, 1, 0, 0, 0, 0, 0, 0};
	data.map[9]  = (int []){0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	data.map[10] = (int []){0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	data.map[11] = (int []){0, 0, 0, 1, 1, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0};
	data.map[12] = (int []){0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	data.map[13] = (int []){0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0};
	data.map[14] = (int []){0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0};
	if (argc != 2)
		return (printf("Error\nWrong number of arguments!\n"), 1);
	data.in.fd = -1;
	data.in.input = NULL;
	init_data(&data, argv[1]);
	mlx_set_cursor_mode(data.mlx, MLX_MOUSE_DISABLED);
	mlx_focus(data.mlx);
	if (mlx_image_to_window(data.mlx, data.win, 0, 0) == -1)
		destroy_data(&data, &data.in, 1, "Failed to draw img to window");
	if (mlx_image_to_window(data.mlx, data.win_entities, 0, 0) == -1)
		destroy_data(&data, &data.in, 1, "Failed to draw img to window");
	if (mlx_image_to_window(data.mlx, data.mm_win, 0, 0) == -1)
		destroy_data(&data, &data.in, 1, "Failed to draw img to window");
	if (mlx_image_to_window(data.mlx, data.mm_txt, 0, 0) == -1)
		destroy_data(&data, &data.in, 1, "Failed to draw img to window");
	mlx_loop_hook(data.mlx, ft_loop_hook, &data);
	// mlx_scroll_hook(data.mlx, ft_scroll_hook, &data);
	mlx_key_hook(data.mlx, ft_key_hook, &data);
	mlx_resize_hook(data.mlx, ft_resize_hook, &data);
	mlx_mouse_hook(data.mlx, ft_mouse_hook, &data);
	ft_create_minimap(&data);
	mlx_loop(data.mlx);
	destroy_data(&data, &data.in, 0, NULL);
	return (printf("Exited!\n"), 0);
}
