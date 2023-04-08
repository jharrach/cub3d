/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:48:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/08 04:12:41 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

static void	draw_pixel(mlx_image_t *img, t_vec2i p, uint32_t col)
{
	const int32_t	i = (p.x + p.y * img->width);

	if (p.x >= 0 && p.x < (int32_t)img->width
		&& p.y >= 0 && p.y < (int32_t)img->height)
		((uint32_t *)img->pixels)[i] = col;
}

bool	ft_button(mlx_t *mlx, mlx_image_t *win, mlx_texture_t *tex)
{
	int	i;
	int	j;
	t_vec2i const	delta = {.x = (int32_t)(win->width - tex->width) / 2, .y = (int32_t)(win->height - tex->height) / 2};
	t_vec2i	mouse;
	bool	col;

	col = false;
	mlx_get_mouse_pos(mlx, &mouse.x, &mouse.y);
	i = 0;
	while (i < (int)tex->height)
	{
		j = 0;
		while (j < (int)tex->width)
		{
			if (delta.x + j == mouse.x && delta.y + i == mouse.y)
				col = true;
			draw_pixel(win, (t_vec2i){.x = delta.x + j, .y = delta.y + i}, ((uint32_t *)(tex->pixels))[i * tex->width + j]);
			j++;
		}
		i++;
	}
	printf("%d %d\n", col, delta.x);
	return (col);
}

static void	fill(mlx_image_t *img, uint32_t col)
{
	int32_t	i;

	i = img->width * img->height;
	while (i--)
		((uint32_t *)img->pixels)[i] = col;
}

void	ft_menu(t_data *data)
{
	static bool over_button = false;
	t_vec2i	mouse;

	data->mm_img->enabled = false;
	data->mm_win->enabled = false;
	scale_texture_to_img(data->background, data->win);
	fill(data->win_entities, 0x0);
	if (data->started)
	{
		over_button = ft_button(data->mlx, data->win_entities, data->button[over_button + 2]);
		if (over_button && mlx_is_mouse_down(data->mlx, MLX_MOUSE_BUTTON_LEFT))
			mlx_close_window(data->mlx);
	}
	else
	{
		over_button = ft_button(data->mlx, data->win_entities, data->button[over_button]);
		if (over_button && mlx_is_mouse_down(data->mlx, MLX_MOUSE_BUTTON_LEFT))
		{
			data->menu = false;
			data->mm_img->enabled = true;
			data->mm_win->enabled = true;
			mlx_set_cursor_mode(data->mlx, MLX_MOUSE_DISABLED);
			data->started = true;
			mlx_get_mouse_pos(data->mlx, &mouse.x, &mouse.y);
			data->dir_delta = mouse.x * -MOUSE_MUL;
		}
	}
}

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
