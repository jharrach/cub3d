/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_s.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 18:37:30 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:05:17 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static void	ft_handleflags(const char *s1, t_placeholder *ph)
{
	ph->len = ft_strlen(s1);
	if (ph->flags & F_DOT && ph->precision < ph->len)
		ph->len = ph->precision;
	if (ph->width < ph->len)
		ph->width = ph->len;
}

int	ft_printf_s(const char *s1, t_placeholder ph)
{
	const char	null[] = "(null)";
	char		*s;
	int			i;

	if (!s1)
		s1 = null;
	ft_handleflags(s1, &ph);
	s = (char *)malloc(sizeof(char) * ph.width);
	if (!s)
		return (0);
	ft_memset(s, ' ', ph.width);
	if (ph.flags & F_MINUS)
		i = ph.width;
	else
	{
		i = ph.len;
		if (ph.flags & F_ZERO)
			ft_memset(s, '0', ph.width);
	}
	ft_memcpy(s + ph.width - i, s1, ph.len);
	write(1, s, ph.width);
	free(s);
	return (ph.width);
}
