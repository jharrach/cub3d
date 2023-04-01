/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.de>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 23:08:59 by rburgsta          #+#    #+#             */
/*   Updated: 2023/03/31 23:08:59 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	destroy_data(t_data *data, bool ext, char *error)
{
	mlx_terminate(data->mlx);
	free(data->ray_lenghts);
	if (data->in.fd != -1)
		close(data->in.fd);
	ft_free2d(data->in.input);
	ft_free2d((char **)data->map);
	if (error != NULL)
		printf("Error\n%s\n", error);
	if (ext == true)
		exit(1);
}
