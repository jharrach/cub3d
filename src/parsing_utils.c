/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 23:09:05 by rburgsta          #+#    #+#             */
/*   Updated: 2023/03/31 23:09:05 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_free2d(char **arr)
{
	int	i;

	i = 0;
	while (arr != NULL && arr[i] != NULL)
		free(arr[i++]);
	free(arr);
}

const char	*get_ident(int i)
{
	const char	*texture[TEXTURE_CNT] = {"NO", "SO", "EA", "WE", "DO", \
		"E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "BG", "MM", \
		"B1", "B2", "B3", "B4"};

	if (i < 0 || i > TEXTURE_CNT - 1)
		return (NULL);
	return (texture[i]);
}

t_input_types	get_input_type(char *line)
{
	int	i;

	if (line == NULL)
		return (INVALID);
	if (ft_strlen(line) == 0)
		return (NEWLINE);
	i = -1;
	while (++i < TEXTURE_CNT)
		if (!ft_strncmp(get_ident(i), line, 2))
			return (TEXTURE);
	if (!ft_strncmp("F", line, 1) || !ft_strncmp("C", line, 1))
		return (COLOR);
	return (INVALID);
}

int	cnt_spaces(char *str)
{
	size_t	i;

	i = 0;
	while (str[i] == ' ' && str[i] != '\0')
		i++;
	return (i);
}

int	check_ints(char **str)
{
	int		i;
	size_t	i2;

	i = -1;
	if (str == NULL)
		return (1);
	while (str[++i] != NULL)
	{
		if ((ft_strlen(str[i]) > 3) || *str[i] == '\0')
			return (ft_free2d(str), 1);
		i2 = -1;
		while (++i2 < ft_strlen(str[i]))
			if (str[i][i2] < '0' || str[i][i2] > '9')
				return (ft_free2d(str), 1);
		i2 = ft_atoi(str[i]);
		if (i2 > 255)
			return (ft_free2d(str), 1);
	}
	if (i != 3)
		return (ft_free2d(str), 1);
	return (0);
}
