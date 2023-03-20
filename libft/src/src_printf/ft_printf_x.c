/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_x.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:53:11 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:05:22 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static char	ft_nbrlen(unsigned int num)
{
	char	len;

	len = 0;
	if (!num)
		len++;
	while (num)
	{
		len++;
		num /= 16;
	}
	return (len);
}

static int	ft_handleflags(unsigned int num, t_placeholder *ph)
{
	int	i;

	ph->len = ft_nbrlen(num);
	if (!num && ph->flags & F_DOT && !ph->precision)
		ph->len = 0;
	if (ph->precision > ph->len)
		ph->len = ph->precision;
	if (ph->flags & F_HASH)
		ph->len += 2;
	if (ph->width < ph->len)
		ph->width = ph->len;
	i = ph->len;
	if (ph->flags & F_MINUS)
		ph->len = ph->width;
	else
	{
		i = ph->width;
		if (ph->flags & F_ZERO && !(ph->flags & F_DOT))
		{
			ph->precision = ph->width;
			ph->len = ph->width;
		}
	}
	return (i);
}

static void	ft_createstr(char *s, unsigned int num, int i, t_placeholder ph)
{
	ft_memset(s, ' ', ph.width);
	if (ph.precision)
	{
		if (ph.flags & F_HASH && !(ph.flags & F_ZERO && !(ph.flags & F_DOT)))
			ft_memset(s + ph.width - ph.len + 2, '0', ph.precision);
		else
			ft_memset(s + ph.width - ph.len, '0', ph.precision);
	}
	if (!num && !(ph.flags & F_DOT && !ph.precision))
		s[--i] = '0';
	while (num)
	{
		s[--i] = num % 16 + '0';
		num /= 16;
		if (s[i] > '9')
			s[i] += ph.type - 81;
	}
	if (ph.flags & F_HASH)
	{
		s[ph.width - ph.len + 1] = ph.type;
		s[ph.width - ph.len] = '0';
	}
}

int	ft_printf_x(unsigned int num, t_placeholder ph)
{
	char	*s;
	int		i;

	if (!num)
		ph.flags &= ~F_HASH;
	i = ft_handleflags(num, &ph);
	s = malloc(sizeof(char) * ph.width);
	if (!s)
		return (0);
	ft_createstr(s, num, i, ph);
	write (1, s, ph.width);
	free(s);
	return (ph.width);
}
