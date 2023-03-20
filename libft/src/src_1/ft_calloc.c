/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 22:35:10 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:02:50 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*p;

	if (count && (size * count) / count != size)
		return (NULL);
	p = (char *)malloc(size * count);
	if (!p)
		return (0);
	ft_bzero(p, size * count);
	return ((void *)p);
}
