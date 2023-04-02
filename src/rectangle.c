/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rectangle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:01:43 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/01 16:24:13 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void	init_recti_center_vec2f(t_recti *rect, t_vec2f center, float halfwidth)
{
	rect->tr.x = floorf(center.x + halfwidth);
	rect->tr.y = floorf(center.y + halfwidth);
	rect->tl.x = floorf(center.x - halfwidth);
	rect->tl.y = floorf(center.y + halfwidth);
	rect->br.x = floorf(center.x + halfwidth);
	rect->br.y = floorf(center.y - halfwidth);
	rect->bl.x = floorf(center.x - halfwidth);
	rect->bl.y = floorf(center.y - halfwidth);
}

void	init_rectf_center_vec2f(t_rectf *rect, t_vec2f center, float halfwidth)
{
	rect->tr.x = center.x + halfwidth;
	rect->tr.y = center.y + halfwidth;
	rect->tl.x = center.x - halfwidth;
	rect->tl.y = center.y + halfwidth;
	rect->br.x = center.x + halfwidth;
	rect->br.y = center.y - halfwidth;
	rect->bl.x = center.x - halfwidth;
	rect->bl.y = center.y - halfwidth;
}

bool	recti_collide_map(t_recti *rect, int **map, t_vec2i map_size)
{
	if (rect->tr.x >= 0 && rect->tr.x < map_size.x && \
			rect->tr.y >= 0 && rect->tr.y < map_size.y && \
			map[rect->tr.x][rect->tr.y] > 0)
		return (true);
	if (rect->tl.x >= 0 && rect->tl.x < map_size.x && \
			rect->tl.y >= 0 && rect->tl.y < map_size.y && \
			map[rect->tl.x][rect->tl.y] > 0)
		return (true);
	if (rect->br.x >= 0 && rect->br.x < map_size.x && \
			rect->br.y >= 0 && rect->br.y < map_size.y && \
			map[rect->br.x][rect->br.y] > 0)
		return (true);
	if (rect->bl.x >= 0 && rect->bl.x < map_size.x && \
			rect->bl.y >= 0 && rect->bl.y < map_size.y && \
			map[rect->bl.x][rect->bl.y] > 0)
		return (true);
	return (false);
}
