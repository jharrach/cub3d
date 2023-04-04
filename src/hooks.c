/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:48:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/02 23:34:30 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	ft_loop_hook(void *param)
{
	t_data *const	data = param;

	ft_fps(data);
	get_mouse_input(data);
	get_key_input(data);
	draw_rectangle(data->win, (t_vec2i){0, 0}, \
		(t_vec2i){data->win->width, data->win->height / 2}, 0xFF00FF00);
	draw_rectangle(data->win, (t_vec2i){0, data->win->height / 2}, \
		(t_vec2i){data->win->width, data->win->height / 2}, 0xFFFF0000);
	ft_rays(data);
	ft_entities(data);
	ft_door(data);
	draw_minimap(data);
}

// void	ft_scroll_hook(double xdelta, double ydelta, void *param)
// {
// 	t_data *const	data = param;

// 	(void)(xdelta);
// 	data->fov += ydelta * 0.01;
// 	if (data->fov < PI / 6.0 || data->fov > PI * 2.0 / 3.0)
// 		data->fov -= ydelta * 0.01;
// 	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
// 	update_ray_angles(data);
// }

void	ft_resize_hook(int32_t width, int32_t height, void *param)
{
	t_data *const	data = param;

	if (!mlx_resize_image(data->win, width, height))
		destroy_data(data, true, "mlx_resize_image()");
	if (!mlx_resize_image(data->win_entities, width, height))
		destroy_data(data, true, "mlx_resize_image()");
	if (!mlx_resize_image(data->mm_win, data->win->width / 8, data->win->width / 8))
		destroy_data(data, true, "mlx_resize_image()");
	data->mm_win_h.x = data->mm_win->width / 2;
	data->mm_win_h.y = data->mm_win->height / 2;
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	ft_free(data, data->ray_lenghts);
	data->ray_lenghts = ft_alloc(data, data->win->width, sizeof(*(data->ray_lenghts)));
	ft_free(data, data->ray_angle);
	data->ray_angle = ft_alloc(data, data->win->width, sizeof(*(data->ray_angle)));
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
	if (keydata.key == MLX_KEY_F && keydata.action == MLX_RELEASE)
		ft_check_door(data);
}

void	ft_mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	t_data *const	data = param;

	(void)mods;
	if (button == MLX_MOUSE_BUTTON_RIGHT)
	{
		if (action == MLX_RELEASE)
		{
			data->fov += AIM_ZOOM;
			if (data->fov < PI / 6.0 || data->fov > PI * 2.0 / 3.0)
				data->fov -= AIM_ZOOM;
		}
		else if (action == MLX_PRESS)
		{
			data->fov -= AIM_ZOOM;
			if (data->fov < PI / 6.0 || data->fov > PI * 2.0 / 3.0)
				data->fov += AIM_ZOOM;
		}
		data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
		update_ray_angles(data);
	}
}
