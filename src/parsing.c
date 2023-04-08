/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 13:57:29 by rburgsta          #+#    #+#             */
/*   Updated: 2023/04/01 13:57:29 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static int	load_color(t_data *data, uint32_t *dest)
{
	int		i;
	int		i2;
	char	**values;

	i = 0;
	i2 = 0;
	while ((*data->in.i)[i])
		if ((*data->in.i)[i++] == ',')
			i2++;
	if (i2 != 2)
		return (printf("Error\nInvalid color format!\n"), 1);
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
	if (get_input_type(*in->i) != COLOR
		|| get_input_type(*(in->i + 1)) != COLOR)
		return (printf("Error\nMissing colors!\n"), 1);
	if (!ft_strncmp("F", *in->i, 1))
	{
		if (load_color(data, &data->col_floor) \
			|| load_color(data, &data->col_ceiling))
			destroy_data(data, 1, NULL);
	}
	else if (load_color(data, &data->col_ceiling) \
		|| load_color(data, &data->col_floor))
		destroy_data(data, 1, NULL);
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

// Lines:
// if (ret != NULL && ft_strlen(ret) > 0 && ret[ft_strlen(ret) - 1] == '\r')
// 	ret[ft_strlen(ret) - 1] = 0;
// only needed for windows
static void	read_input(t_data *data, t_input *in)
{
	char	*ret;
	int		i;
	int		i2;

	i = 0;
	ret = (void *)1;
	while (ret != NULL)
	{
		in->i = ft_alloc(data, (i + 1), sizeof(char *));
		i2 = -1;
		while (++i2 < i - 1)
			in->i[i2] = in->input[i2];
		if (i++ > 0)
			in->i[i2++] = ret;
		in->i[i2] = NULL;
		if (in->input != NULL)
			ft_free(data, in->input);
		in->input = in->i;
		ret = ft_alloc_add(data, get_next_line(in->fd));
		if (ret != NULL && ft_strlen(ret) && ret[ft_strlen(ret) - 1] == '\n')
			ret[ft_strlen(ret) - 1] = 0;
		if (ret != NULL && ft_strlen(ret) && ret[ft_strlen(ret) - 1] == '\r')
			ret[ft_strlen(ret) - 1] = 0;
	}
}

void	load_data(t_data *data, t_input *in)
{
	read_input(data, in);
	if (get_input_type(*in->i) == TEXTURE)
	{
		if (load_textures(data, in) || load_colors(data, in))
			destroy_data(data, 1, NULL);
	}
	else if (load_colors(data, in) || load_textures(data, in))
		destroy_data(data, 1, NULL);
	while (*in->i != *in->input && get_input_type(*in->i) == NEWLINE)
		in->i++;
	if (*in->i == NULL)
		destroy_data(data, 1, "No map found!");
	load_map(data, in);
}
