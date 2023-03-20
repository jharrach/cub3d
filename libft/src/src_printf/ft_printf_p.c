/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_p.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 16:56:08 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:05:14 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static char	ft_p_len(uintptr_t p)
{
	char	len;

	len = 0;
	if (!p)
		len++;
	while (p)
	{
		len++;
		p /= 16;
	}
	return (len);
}

static void	ft_createstr(char *s, uintptr_t p, t_placeholder ph)
{
	int	i;

	ft_memset(s, ' ', ph.width);
	if (ph.flags & F_MINUS)
		i = ph.len;
	else
	{
		i = ph.width;
		if (ph.flags & F_ZERO && !(ph.flags & F_DOT))
		{
			ft_memset(s, '0', ph.width);
			ph.precision = ph.width - ph.len;
		}
	}
	if (!p)
		s[--i] = '0';
	while (p)
	{
		s[--i] = p % 16 + '0';
		p /= 16;
		if (s[i] > '9')
			s[i] += 39;
	}
	s[--i - ph.precision] = 'x';
	s[--i - ph.precision] = '0';
}

int	ft_printf_p(uintptr_t p, t_placeholder ph)
{
	char	*s;

	ph.len = ft_p_len(p) + 2;
	ph.precision = 0;
	if (ph.width < ph.len)
		ph.width = ph.len;
	s = (char *)malloc(ph.width);
	if (!s)
		return (0);
	ft_createstr(s, p, ph);
	write(1, s, ph.width);
	free(s);
	return (ph.width);
}
