#include "../include/cub3d.h"

static int	load_color(t_data *data, uint32_t *dest)
{
	int		i;
	char	**values;

	if ((*data->in.i)[ft_strlen(*data->in.i) - 1] == '\n')
		(*data->in.i)[ft_strlen(*data->in.i) - 1] = 0;
	if ((*data->in.i)[ft_strlen(*data->in.i) - 1] == '\r') //Only windows
		(*data->in.i)[ft_strlen(*data->in.i) - 1] = 0; //Only windows
	i = cnt_spaces(*data->in.i + 1) + 1;
	values = ft_split(*data->in.i + i, ',');
	if (check_ints(values))
		return (printf("Error\nInvalid color format!\n"), 1);
	*dest = 255 << 24 | ft_atoi(values[0]) \
		| ft_atoi(values[1]) << 8 | ft_atoi(values[2]) << 16;
	ft_free2d(values);
	data->in.i++;
	return (0);
}

static int	load_colors(t_data *data, t_input *in)
{
	while (*in->i != *in->input && get_input_type(*in->i) == NEWLINE)
		in->i++;
	if (*in->i == NULL || get_input_type(*in->i) != COLOR
		|| get_input_type(*(in->i + 1)) != COLOR)
		return (printf("Error\nMissing colors!\n"), 1);
	if (!ft_strncmp("F", *in->i, 1))
	{
		if (load_color(data, &data->col_floor) \
			|| load_color(data, &data->col_ceiling))
			destroy_data(data, in, 1, NULL);
	}
	else if (load_color(data, &data->col_ceiling) \
		|| load_color(data, &data->col_floor))
		destroy_data(data, in, 1, NULL);
	return (0);
}

static int	load_textures(t_data *data, t_input *in)
{
	int	i;

	while (*in->i != *in->input && get_input_type(*in->i) == NEWLINE)
		in->i++;
	while (get_input_type(*in->i) == TEXTURE)
	{
		i = 0;
		while (i < TEXTURE_CNT && ft_strncmp(get_ident(i), *in->i, 2))
			i++;
		if ((*in->i)[ft_strlen(*in->i) - 1] == '\n')
			(*in->i)[ft_strlen(*in->i) - 1] = 0;
		if ((*in->i)[ft_strlen(*in->i) - 1] == '\r') //Only windows
			(*in->i)[ft_strlen(*in->i) - 1] = 0; //Only windows
		if (data->texture[i] != NULL)
			return (printf("Error\nTexture duplicate input file!\n"), 1);
		data->texture[i] = mlx_load_png(*in->i + cnt_spaces(*in->i + 2) + 2);
		if (data->texture[i] == NULL)
			return (printf("Error\nTexture load fail!\n"), 1);
		in->i++;
	}
	i = -1;
	while (++i < TEXTURE_CNT)
		if (data->texture[i] == NULL)
			return (printf("Error\nMissing textures!\n"), 1);
	return (0);
}

static int	read_input(t_input *in)
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
	if (get_input_type(*in->i) == TEXTURE)
	{
		if (load_textures(data, in) || load_colors(data, in))
			destroy_data(data, in, 1, NULL);
	}
	else if (load_colors(data, in) || load_textures(data, in))
		destroy_data(data, in, 1, NULL);
	load_map(data, in);
}
