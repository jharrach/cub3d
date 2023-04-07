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
	ft_lstclear(&data->head, &free);
}

int main()
{
	t_data data = (t_data){0};

	int *tests[100];
	int i = -1;

	char *t = malloc(200);
	ft_alloc_add(&data, t);
	while (++i < 100)
		tests[i] = ft_alloc(&data, 100, 1000);
	// while (i > 0)
	// 	ft_free(&data, tests[--i]);
	ft_lstclear(&data.head, &free);
	return (0);
}
