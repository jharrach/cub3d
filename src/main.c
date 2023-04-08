/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 22:39:47 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/08 18:02:05 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

int	main(int argc, char **argv)
{
	t_data	data;

	if (argc != 2)
		return (printf("Error\nWrong number of arguments!\n"), 1);
	data.in.fd = -1;
	data.in.input = NULL;
	init_data(&data, argv[1]);
	mlx_focus(data.mlx);
	if (mlx_image_to_window(data.mlx, data.win, 0, 0) == -1)
		destroy_data(&data, 1, "Failed to draw img to window");
	if (mlx_image_to_window(data.mlx, data.win_entities, 0, 0) == -1)
		destroy_data(&data, 1, "Failed to draw img to window");
	if (mlx_image_to_window(data.mlx, data.mm_win, 0, 0) == -1)
		destroy_data(&data, 1, "Failed to draw img to window");
	if (mlx_image_to_window(data.mlx, data.mm_img, 0, 0) == -1)
		destroy_data(&data, 1, "Failed to draw img to window");
	mlx_loop_hook(data.mlx, ft_loop_hook, &data);
	mlx_scroll_hook(data.mlx, ft_scroll_hook, &data);
	mlx_key_hook(data.mlx, ft_key_hook, &data);
	mlx_resize_hook(data.mlx, ft_resize_hook, &data);
	mlx_loop(data.mlx);
	destroy_data(&data, 0, NULL);
	return (0);
}
