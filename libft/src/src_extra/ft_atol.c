/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 13:46:05 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 13:54:43 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

long	ft_atol(const char *str)
{
	unsigned long	result;
	char			c;
	int				neg;

	c = ' ';
	while (ft_isspace(c))
		c = *str++;
	neg = c == '-';
	if (neg || c == '+')
		c = *str++;
	result = 0;
	while (c >= '0' && c <= '9')
	{
		result = 10 * result + c - '0';
		c = *str++;
		if (!neg && result > (unsigned long)LONG_MAX)
			return (LONG_MAX);
		if (neg && result > (unsigned long)LONG_MIN)
			return (LONG_MIN);
	}
	if (neg)
		return (-(long)result);
	else
		return (result);
}
