/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf_u.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 21:43:22 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/14 21:13:21 by jharrach         ###   ########.fr       */
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
		num /= 10;
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

int	ft_dprintf_u(int fd, unsigned int num, t_placeholder ph)
{
	char	*s;
	int		i;

	i = ft_handleflags(num, &ph);
	s = (char *)malloc(sizeof(char) * ph.width);
	if (!s)
		return (0);
	ft_memset(s, ' ', ph.width);
	if (ph.precision)
		ft_memset(s + ph.width - ph.len, '0', ph.precision);
	if (!num && !(ph.flags & F_DOT && !ph.precision))
		s[--i] = '0';
	while (num)
	{
		s[--i] = num % 10 + 48;
		num /= 10;
	}
	write(fd, s, ph.width);
	free(s);
	return (ph.width);
}
