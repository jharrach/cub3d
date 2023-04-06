/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 17:12:14 by rburgsta          #+#    #+#             */
/*   Updated: 2023/04/06 19:09:45 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	destroy_data(t_data *data, bool ext, char *error)
{
	return ;
}

int main()
{
	t_data data = (t_data){0};

	data.ray_angle = ft_alloc(&data, 5, sizeof(float));
	data.ray_lenghts = ft_alloc(&data, 5, sizeof(float));

	// ft_free(&data, data.ray_angle);
	// ft_free(&data, data.ray_lenghts);
	ft_lstclear(&data.head, &free);
	return (0);
}
