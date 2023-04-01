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

static int	set_map_size(t_data *data, t_input *in)
{
	int		i;
	size_t	stop;
	size_t	start;

	i = -1;
	start = -1;
	stop = 0;
	while (in->i[++i] != NULL)
	{
		if (get_input_type(in->i[i]) == NEWLINE)
			destroy_data(data, 1, "Newline in map!");
		if (in->i[i][ft_strlen(in->i[i]) - 1] == '\n')
			in->i[i][ft_strlen(in->i[i]) - 1] = 0;
		if (in->i[i][ft_strlen(in->i[i]) - 1] == '\r') // Only windows
			in->i[i][ft_strlen(in->i[i]) - 1] = 0; // Only windows
		if (start > (size_t)cnt_spaces(in->i[i]))
			start = cnt_spaces(in->i[i]);
		if (stop < ft_strlen(in->i[i]))
			stop = ft_strlen(in->i[i]);
	}
	data->map_size = (t_vec2i){i, stop - start};
	printf("%i %i\n", data->map_size.x, data->map_size.y);
	return (start);
}

void	allocate_map_arr(t_data *data)
{
	int	i;

	data->map = ft_calloc(data->map_size.x, sizeof(*(data->map)));
	if (data->map == NULL)
		destroy_data(data, 1, "Failed to allocate map!");
	i = -1;
	while (++i < data->map_size.x)
	{
		data->map[i] = malloc(sizeof(int) * data->map_size.y);
		if (data->map[i] == NULL)
			destroy_data(data, 1, "Failed to allocate map!");
	}
}

void	set_player_data(t_data *data, char c, int y, int x)
{
	int	i;

	if (data->pos.x == -1)
	{
		printf("%i %i\n", y ,x);
		data->pos = (t_vec2f){x + 0.5, y + 0.5};
		i = -1;
		while (++i < 4)
			if (c == "NESW"[i])
				data->dir = i * PI / 2;
		printf("%f %f %f\n", data->pos.x ,data->pos.y, data->dir / (PI / 180));
	}
	else
		destroy_data(data, 1, "Multiple players in map!");
}

void	load_map(t_data *data, t_input *in)
{
	int	x;
	int	y;
	int	start;

	start = set_map_size(data, in);
	allocate_map_arr(data);
	y = -1;
	data->pos = (t_vec2f){-1, -1};
	while (++y < data->map_size.x)
	{
		x = start - 1;
		while (++x < data->map_size.y + start)
		{
			if (x >= (int)ft_strlen(in->i[y]) || in->i[y][x] == ' ')
				data->map[y][x - start] = 0;
			else if (ft_strchr(" 0123NSEW", in->i[y][x]) == NULL)
				destroy_data(data, 1, "Invalid character(s) in map!");
			else if (ft_strchr("NSEW", in->i[y][x]) != NULL)
				set_player_data(data, in->i[y][x], y, x);
			else
				data->map[y][x - start] = in->i[y][x] - '0';
		}
	}
	// for (int y = 0; y < data->map_size.x; y++)
	// {
	// 	for (int x = 0; x <  data->map_size.y; x++)
	// 	{
	// 		printf("%i", data->map[y][x]);
	// 	}
	// 	printf("\n");
	// }
	if (data->pos.x == -1)
		destroy_data(data, 1, "No player in map!");
}
