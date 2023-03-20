/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_containsdigit.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 13:47:02 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 13:55:44 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

int	ft_containsdigit(const char *str)
{
	char	c;

	c = *str++;
	while (c)
	{
		if (ft_isdigit(c))
			return (1);
		c = *str++;
	}
	return (0);
}
