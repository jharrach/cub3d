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
	int	i;

	i = -1;
	while (++i < TEXTURE_CNT)
		if (data->texture[i] != NULL)
			mlx_delete_texture(data->texture[i]);
	if (data->mlx != NULL)
		mlx_terminate(data->mlx);
	if (data->in.fd != -1)
		close(data->in.fd);
	ft_lstclear(&data->head, &free);
	if (error != NULL)
		printf("Error\n%s\n", error);
	*data = (t_data){0};
	if (ext == true)
		exit(1);
}
