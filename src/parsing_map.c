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

static t_vec2i	get_map_size(t_data *data, t_input *in)
{
	int	i;
	int	i2;
	int	start;
	int	stop;

	i = -1;
	start = 0;
	stop = 0;
	while (in->i[++i] != NULL)
	{
		if (get_input_type(in->i[i]) == NEWLINE)
			destroy_data(data, &data->in, 1, "Newline in map!");
		if (in->i[i][ft_strlen(in->i[i]) - 1] == '\n')
			in->i[i][ft_strlen(in->i[i]) - 1] = 0;
		if (in->i[i][ft_strlen(in->i[i]) - 1] == '\r') // Only windows
			in->i[i][ft_strlen(in->i[i]) - 1] = 0; // Only windows
		if (start > cnt_spaces(in->i[i]))
			start = cnt_spaces(in->i[i]);
		i2 = cnt_spaces(in->i[i]);
		while (ft_isalnum(in->i[i][i2]))
			i2++;
		if (stop < i2)
			stop = ft_strlen(in->i[i]);
	}
	return ((t_vec2i){i, stop - start});
}

void	allocate_map(t_data *data, t_input *in)
{
	int	i;
	int	i2;

	data->map = malloc(sizeof(*(data->map)) * data->map_size.x);
	i = -1;
	while (++i < data->map_size.x)
		data->map[i] = malloc(sizeof(int) * data->map_size.y);
	i = -1;
	while (++i < data->map_size.x)
	{
		i2 = -1;
		while (++i2 < data->map_size.y)
		{
			if (i2 < (int)ft_strlen(in->i[i]))
			{
				if (in->i[i][i2] == 'N')
					data->pos = (t_vec2f){i + 0.5, i2 + 0.5};
				if (in->i[i][i2] == ' ' || in->i[i][i2] == 'N')
					data->map[i][i2] = 0;
				else
					data->map[i][i2] = in->i[i][i2] - '0';
			}
			else
				data->map[i][i2] = 0;
		}
	}
	// for (int y = 0; y < data->map_size.x; y++)
	// {
	// 	for (int x = 0; x <  data->map_size.y; x++)
	// 	{
	// 		printf("'%i'", data->map[y][x]);
	// 	}
	// 	printf("\n");
		
	// }
	// int temp = data->map_size.x;
	// data->map_size.x = data->map_size.y;
	// data->map_size.y = temp;
	
}

void	load_map(t_data *data, t_input *in)
{
	while (*in->i != *in->input && get_input_type(*in->i) == NEWLINE)
		in->i++;
	data->map_size = get_map_size(data, in);
	allocate_map(data, in);
}
