/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/01 16:02:03 by jharrach          #+#    #+#             */
/*   Updated: 2023/04/09 01:45:13 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

t_vec2f	vec2f_mul_f(t_vec2f v, float m)
{
	v.x *= m;
	v.y *= m;
	return (v);
}

t_vec2f	rotate_vec2f(t_vec2f v1, float angle)
{
	t_vec2f	v2;

	v2.x = v1.x * cosf(angle) - v1.y * sinf(angle);
	v2.y = v1.x * sinf(angle) + v1.y * cosf(angle);
	return (v2);
}
