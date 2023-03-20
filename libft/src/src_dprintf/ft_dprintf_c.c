/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf_c.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 18:35:52 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/14 21:12:35 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

int	ft_dprintf_c(int fd, char c, t_placeholder ph)
{
	char	*s;

	if (ph.width <= 1)
		return (write(fd, &c, 1));
	s = (char *)malloc(sizeof(char) * ph.width);
	if (!s)
		return (0);
	if (ph.flags & F_MINUS)
	{
		ft_memset(s, ' ', ph.width);
		*s = c;
	}
	else
	{
		if (ph.flags & F_ZERO)
			ft_memset(s, '0', ph.width);
		else
			ft_memset(s, ' ', ph.width);
		s[ph.width - 1] = c;
	}
	write(fd, s, ph.width);
	free(s);
	return (ph.width);
}
