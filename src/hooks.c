/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:48:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/06 16:28:47 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_loop_hook(void *param)
{
	t_data *const	data = param;

	ft_fps(data);
	get_mouse_input(data);
	get_key_input(data);
	if (data->menu)
	{
		if (data->collected == data->num_entities)
		{
			data->collected++;
			data->won = mlx_put_string(data->mlx, "YOU WON!\n", 0, 0);
			mlx_image_to_window(data->mlx, data->won, data->win->width - data->won->width, data->win->height - data->won->height);
			mlx_set_cursor_mode(data->mlx, MLX_MOUSE_NORMAL);
		}
		return ;
	}
	draw_rectangle(data->win, (t_vec2i){0, 0}, \
		(t_vec2i){data->win->width, data->win->height / 2}, \
		data->col_ceiling);
	draw_rectangle(data->win, (t_vec2i){0, data->win->height / 2.0}, \
		(t_vec2i){data->win->width, ceilf((float)data->win->height / 2.0)}, \
		data->col_floor);
	ft_rays(data);
	ft_entities(data);
	ft_door(data);
	draw_minimap(data);
}

void	ft_scroll_hook(double xdelta, double ydelta, void *param)
{
	t_data *const	data = param;

	(void)(xdelta);
	data->fov += ydelta * 0.01;
	if (data->fov < PI / 6.0 || data->fov > PI * 2.0 / 3.0)
		data->fov -= ydelta * 0.01;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	update_ray_angles(data);
}

void	ft_resize_hook(int32_t width, int32_t height, void *param)
{
	t_data *const	data = param;

	if (!mlx_resize_image(data->win, width, height))
		destroy_data(data, true, "mlx_resize_image()");
	if (!mlx_resize_image(data->win_entities, width, height))
		destroy_data(data, true, "mlx_resize_image()");
	if (!mlx_resize_image(data->mm_win, \
			data->win->width / 8, data->win->width / 8))
		destroy_data(data, true, "mlx_resize_image()");
	if (!mlx_resize_image(data->mm_img, \
			data->win->width / 8, data->win->width / 8))
		destroy_data(data, true, "mlx_resize_image()");
	scale_texture_to_img(data->mm_txt, data->mm_img);
	data->mm_win_h.x = data->mm_win->width / 2;
	data->mm_win_h.y = data->mm_win->height / 2;
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	free(data->ray_lenghts);
	data->ray_lenghts = malloc(sizeof(*(data->ray_lenghts)) * data->win->width);
	free(data->ray_angle);
	data->ray_angle = malloc(sizeof(*(data->ray_angle)) * data->win->width);
	if (!data->ray_angle || !data->ray_lenghts)
		destroy_data(data, true, "malloc()");
	update_ray_angles(data);
}

void	ft_key_hook(mlx_key_data_t keydata, void *param)
{
	t_data *const	data = param;
	static bool		mode = false;

	if (keydata.key == MLX_KEY_Q && keydata.action == MLX_RELEASE)
	{
		mode = !mode;
		if (mode)
			mlx_set_cursor_mode(data->mlx, MLX_MOUSE_NORMAL);
		else
		{
			mlx_set_cursor_mode(data->mlx, MLX_MOUSE_DISABLED);
			mlx_focus(data->mlx);
		}
	}
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_RELEASE)
		ft_check_door(data);
}

// void	ft_mouse_hook(mouse_key_t but, action_t act, modifier_key_t _, void *p)
// {
// 	t_data *const	data = p;

// 	(void)_;
// 	if (but == MLX_MOUSE_BUTTON_LEFT && act == MLX_PRESS)
// 	{
// 		if (data->center_ent != -1 && \
// 			data->ray_lenghts[(int)data->win_wh] > data->dis && rand() & 1)
// 		{
// 			data->entity[data->center_ent].enabled = false;
// 		}
// 	}
// }
