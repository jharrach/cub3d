/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entity_collision.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/09 01:10:42 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/09 01:11:26 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

/**
 * r1 < r2
*/
static bool	square_square_collision(t_rectf *r1, t_rectf *r2)
{
	if (r1->tr.x >= r2->tl.x && r1->tr.x <= r2->tr.x && \
		r1->tr.y >= r2->bl.y && r1->tr.y <= r2->tl.y)
		return (true);
	if (r1->tl.x >= r2->tl.x && r1->tl.x <= r2->tr.x && \
		r1->tl.y >= r2->bl.y && r1->tl.y <= r2->tl.y)
		return (true);
	if (r1->br.x >= r2->tl.x && r1->br.x <= r2->tr.x && \
		r1->br.y >= r2->bl.y && r1->br.y <= r2->tl.y)
		return (true);
	if (r1->bl.x >= r2->tl.x && r1->bl.x <= r2->tr.x && \
		r1->bl.y >= r2->bl.y && r1->bl.y <= r2->tl.y)
		return (true);
	return (false);
}

static void	handle_collision(t_data *data)
{
	data->collected++;
	if (data->collected == data->num_entities)
	{
		data->menu = true;
		mlx_set_cursor_mode(data->mlx, MLX_MOUSE_NORMAL);
	}
	draw_rectangle(&((mlx_image_t){\
		.width = data->texture[MM]->width, \
		.height = data->texture[MM]->height, \
		.pixels = data->texture[MM]->pixels}), \
		(t_vec2i){0, data->texture[MM]->height - 7}, \
		(t_vec2i){(float)data->texture[MM]->width * \
		(float)data->collected / (float)data->num_entities, 7}, \
		ENTITY_BAR_COLOR);
	scale_texture_to_img(data->texture[MM], data->mm_img);
}

void	collide_entity(t_data *data)
{
	t_rectf	player;
	int32_t	i;
	bool	coll;

	init_rectf_center_vec2f(&player, data->pos, PLAYER_HALF_WIDTH);
	i = -1;
	while (++i < data->num_entities)
	{
		if (data->entity[i].del_pos.x > 2.0 || data->entity[i].del_pos.y > 2.0 \
				|| data->entity[i].del_pos.x < -2.0 || \
				data->entity[i].del_pos.y < -2.0 || !data->entity[i].enabled)
			continue ;
		if (data->entity[i].half_width > PLAYER_HALF_WIDTH)
			coll = square_square_collision(&player, &(data->entity[i].rect));
		else
			coll = square_square_collision(&(data->entity[i].rect), &player);
		if (coll)
		{
			data->entity[i].enabled = false;
			handle_collision(data);
		}
	}
}
