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
	return (start);
}

static void	allocate_map_arr(t_data *data)
{
	int	i;

	data->map = ft_alloc(data, data->map_size.x + 1, sizeof(*(data->map)));
	i = -1;
	while (++i < data->map_size.x)
		data->map[i] = ft_alloc(data, data->map_size.y, sizeof(int));
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
				data->dir = i * PI / 2;
	}
	else
		destroy_data(data, 1, "Multiple players in map!");
}

static int **debug_allocate_map(t_data *data)
{
	int	i;
	int **ret;

	ret = ft_alloc(data, data->map_size.x + 1, sizeof(*(data->map)));
	i = -1;
	while (++i < data->map_size.x)
		ret[i] = ft_alloc(data, data->map_size.y, sizeof(int));
	return (ret);
}

t_vec2i	vec2i_add(t_vec2i v1, t_vec2i v2)
{
	return ((t_vec2i){v1.x + v2.x, v1.y + v2.y});
}

void rotate_dir(t_vec2i *loc, int i)
{
	if (loc->x == -1 && loc->y < 1)
		loc->y++;
	else if (loc->y == 1 && loc->x < 1)
		loc->x++;
	else if (loc->x == 1 && loc->y > -1)
		loc->y--;
	else
		loc->x--;
	if (--i > 0)
		rotate_dir(loc, i);
}

static void validate_map(t_data *data)
{
	int		**map;
	t_vec2i	i;
	t_vec2i dir;
	t_vec2i temp;
	t_vec2i start;
	int		max = 0;

	i.x = -1;
	map = debug_allocate_map(data);
	while (++i.x < data->map_size.x)
		ft_memset(map[i.x], 0, data->map_size.y * sizeof(int));
	i = (t_vec2i){0, 0};
	while (i.y < data->map_size.y && data->map[i.x][i.y] != 1)
		i.y++;
	if (i.y >= data->map_size.y)
		destroy_data(data, 1, "Invalid map!");
	map[i.x][i.y] = 1;
	start = i;
	dir = (t_vec2i){-1, -1};
	int arr_border_touched[4] = {1, 0, 0, 0};
	while (true)
	{
		temp = vec2i_add(i, dir);
		if (temp.x >= 0 && temp.y >= 0 && temp.x < data->map_size.x \
			&& temp.y < data->map_size.y && data->map[temp.x][temp.y] == 1)
		{
			max = 0;
			i = temp;
			map[i.x][i.y]++;
			if (i.y == data->map_size.y - 1)
				arr_border_touched[1] = 1;
			if (i.x == data->map_size.x - 1)
				arr_border_touched[2] = 1;
			if (i.y == 0)
				arr_border_touched[3] = 1;
			rotate_dir(&dir, 5);
			if (i.x == start.x && i.y == start.y)
			{
				if (arr_border_touched[0] + arr_border_touched[1] + arr_border_touched[2] + arr_border_touched[3] == 4)
					printf("Test success map valid as of now!\n");
				else
					printf("Test success and map invalid\n");
				break;
			}
		}
		else
		{
			if (max == 8)
			{
				printf("Test success and map invalid\n");
				break ;
			}
			rotate_dir(&dir, 1);
			max++;
		}
		if (i.x == temp.x && i.y == temp.y)
		{
			printf("\n");
			for (int y = data->map_size.x - 1; y >= 0; y--) // Debug
			{
				for (int x = 0; x < data->map_size.y; x++)
				{
					if (map[y][x] == 0)
						printf(" ");
					else
						printf("%c", map[y][x] + '0');
				}
				printf("\n");
			}
		}
		//usleep(10000); // Debug sleep
	}
}

void	load_map(t_data *data, t_input *in)
{
	t_vec2i i;
	int		start;

	start = set_map_size(data, in);
	allocate_map_arr(data);
	i.x = -1;
	while (++i.x < data->map_size.x)
	{
		i.y = start - 1;
		while (++i.y < data->map_size.y + start)
		{
			if (i.y >= (int)ft_strlen(in->i[i.x]) || in->i[i.x][i.y] == ' ')
				data->map[data->map_size.x - i.x - 1][i.y - start] = 0;
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
	// for (int y = data->map_size.x - 1; y >= 0; y--) // Debug
	// {
	// 	for (int x = 0; x < data->map_size.y; x++)
	// 		printf("%i", data->map[y][x]);
	// 	printf("\n");
	// }
	if (data->pos.x == -1)
		destroy_data(data, 1, "No player in map!");
	validate_map(data);
}
