/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:35:39 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/02 23:34:05 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static char	*ft_sitoa(int n, char *s)
{
	char	*p;
	bool	neg;
	unsigned int	n_cpy;

	if (n < 0)
	{
		neg = true;
		n_cpy = n * -1;
	}
	else
	{
		neg = false;
		n_cpy = n;
	}
	p = s + 11;
	*p = 0;
	while (true)
	{
		*--p = n_cpy % 10 + '0';
		n_cpy /= 10;
		if (!n_cpy)
			break ;
	}
	if (neg)
		*--p = '-';
	return (p);
}

void	ft_fps(t_data *data)
{
	static mlx_image_t	*img = NULL;
	static int			count;
	char				s1[12];
	if (count++ < 10)
		return ;
	count = 0;
	mlx_delete_image(data->mlx, img);
	img = mlx_put_string(data->mlx, \
		ft_sitoa(1.0 / data->mlx->delta_time, s1), 0, data->mlx->height - 20);
	if (!img)
		destroy_data(data, 1, "ft_fps putstring fail");
}
