/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_map.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 13:57:37 by rburgsta          #+#    #+#             */
/*   Updated: 2023/04/01 13:57:37 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	validate_map(t_data *data)
{
	int	x;
	int	y;

	if (data->pos.x == -1)
		destroy_data(data, 1, "No player in map!");
	x = -1;
	while (++x < data->map_size.x)
	{
		y = -1;
		while (++y < data->map_size.y)
		{
			if (ft_strchr("023", data->map[x][y] + '0') != NULL)
			{
				if (x == 0 || y == 0 || x >= data->map_size.x - 1 \
					|| y >= data->map_size.y - 1)
					destroy_data(data, 1, "Map border not closed!");
				else if (data->map[x + 1][y] == -2 \
					|| data->map[x - 1][y] == -2 || data->map[x][y + 1] == -2 \
					|| data->map[x][y - 1] == -2)
					destroy_data(data, 1, "Map border not closed!");
			}
		}
	}
}

static void	set_player_data(t_data *data, char c, int x, int y)
{
	int	i;

	if (data->pos.x == -1)
	{
		data->pos = (t_vec2f){x + 0.5, y + 0.5};
		data->map[x][y] = 0;
		i = -1;
		while (++i < 4)
			if (c == "ESWN"[i])
				data->dir_delta = i * PI / 2;
	}
	else
		destroy_data(data, 1, "Multiple players in map!");
}

static void	allocate_map(t_data *data)
{
	int	i;

	data->map = ft_alloc(data, data->map_size.x + 1, sizeof(*(data->map)));
	i = -1;
	while (++i < data->map_size.x)
		data->map[i] = ft_alloc(data, data->map_size.y, sizeof(int));
}

static int	init_map_array(t_data *data, t_input *in)
{
	int		i;
	size_t	i2;
	size_t	stop;
	size_t	start;

	i = -1;
	start = -1;
	stop = 0;
	while (in->i[++i] != NULL)
	{
		if (get_input_type(in->i[i]) == NEWLINE)
			destroy_data(data, 1, "Newline in map!");
		if (start > (size_t)cnt_spaces(in->i[i]))
			start = cnt_spaces(in->i[i]);
		i2 = ft_strlen(in->i[i]) - 1;
		while (i2 > 0 && in->i[i][i2] == ' ')
			i2--;
		if (stop < i2 + 1)
			stop = i2 + 1;
	}
	data->map_size = (t_vec2i){i, stop - start};
	allocate_map(data);
	return (start);
}

void	load_map(t_data *data, t_input *in)
{
	t_vec2i		i;
	const int	start = init_map_array(data, in);

	i.x = -1;
	while (++i.x < data->map_size.x)
	{
		i.y = start - 1;
		while (++i.y < data->map_size.y + start)
		{
			if (i.y >= (int)ft_strlen(in->i[i.x]) || in->i[i.x][i.y] == ' ')
				data->map[data->map_size.x - i.x - 1][i.y - start] = -2;
			else if (ft_strchr(" 0123NSEW", in->i[i.x][i.y]) == NULL)
				destroy_data(data, 1, "Invalid character(s) in map!");
			else if (ft_strchr("NSEW", in->i[i.x][i.y]) != NULL)
				set_player_data(data, in->i[i.x][i.y], \
					data->map_size.x - i.x - 1, i.y - start);
			else
			{
				data->num_entities += (in->i[i.x][i.y] == '3');
				data->map[data->map_size.x - i.x - 1][i.y - start] \
					= in->i[i.x][i.y] - '0';
			}
		}
	}
	validate_map(data);
}
