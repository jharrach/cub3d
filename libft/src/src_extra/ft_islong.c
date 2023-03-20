/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_islong.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 13:47:26 by jharrach          #+#    #+#             */
/*   Updated: 2023/03/08 13:54:52 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

int	ft_islong(const char *str)
{
	unsigned long	result;
	char			c;
	int				neg;

	if (!ft_containsdigit(str))
		return (0);
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
		if ((!neg && result > (unsigned long)LONG_MAX) \
			|| (neg && result > (unsigned long)LONG_MIN))
			return (0);
	}
	while (ft_isspace(c))
		c = *str++;
	return (!c);
}
