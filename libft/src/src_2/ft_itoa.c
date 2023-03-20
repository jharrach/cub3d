/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 16:20:35 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:03:56 by jharrach         ###   ########.fr       */
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

char	*ft_itoa(int n)
{
	char	*s;
	int		len;
	long	n_cpy;

	n_cpy = n;
	len = ft_nbrlen(n);
	s = (char *)malloc(sizeof(char) * (len + 1));
	if (!s)
		return (0);
	s[len] = 0;
	if (n_cpy < 0)
	{
		s[0] = '-';
		n_cpy *= -1;
	}
	else if (!n_cpy)
		s[0] = '0';
	while (len-- && n_cpy)
	{
		s[len] = n_cpy % 10 + 48;
		n_cpy /= 10;
	}
	return (s);
}
