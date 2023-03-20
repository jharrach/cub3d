/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/18 20:24:46 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:04:04 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static int	ft_nbrlen(int n)
{
	int	len;

	len = 0;
	if (n <= 0)
		len++;
	while (n)
	{	
		len++;
		n /= 10;
	}
	return (len);
}

void	ft_putnbr_fd(int n, int fd)
{
	long	n_cpy;
	char	n_len;
	char	s[11];
	int		i;

	i = 0;
	n_cpy = n;
	if (n_cpy < 0)
	{
		n_cpy *= -1;
		s[0] = '-';
	}
	else if (!n_cpy)
		s[0] = '0';
	n_len = ft_nbrlen(n);
	i = n_len;
	while (n_cpy)
	{
		s[--i] = n_cpy % 10 + 48;
		n_cpy /= 10;
	}
	write (fd, s, n_len);
}
