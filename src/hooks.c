/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:48:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/09 02:41:02 by jharrach         ###   ########.fr       */
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
		ft_menu(data);
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
	if (!mlx_resize_image(data->mm.win, \
			data->win->width / 8, data->win->width / 8))
		destroy_data(data, true, "mlx_resize_image()");
	if (!mlx_resize_image(data->mm.win_tex, \
			data->win->width / 8, data->win->width / 8))
		destroy_data(data, true, "mlx_resize_image()");
	scale_texture_to_img(data->texture[MM], data->mm.win_tex);
	data->mm.win_h.x = (float)data->mm.win->width / 2.0;
	data->mm.win_h.y = (float)data->mm.win->height / 2.0;
	data->mm.scale = (float)data->win->width / MM_SCALE;
	data->win_wh = data->win->width / 2;
	data->dis = (float)data->win_wh / tanf(data->fov / 2.0);
	ft_free(data, data->ray_lenghts);
	data->ray_lenghts = ft_alloc(data, data->win->width, \
		sizeof(*(data->ray_lenghts)));
	ft_free(data, data->ray_angle);
	data->ray_angle = ft_alloc(data, data->win->width, \
		sizeof(*(data->ray_angle)));
	update_ray_angles(data);
}

void	ft_key_hook(mlx_key_data_t keydata, void *param)
{
	t_data *const	data = param;

	if (keydata.key == MLX_KEY_Q && keydata.action == MLX_RELEASE && \
		!data->menu)
	{
		data->menu = true;
		mlx_set_cursor_mode(data->mlx, MLX_MOUSE_NORMAL);
	}
	if (keydata.key == MLX_KEY_SPACE && keydata.action == MLX_RELEASE)
		ft_check_door(data);
}
