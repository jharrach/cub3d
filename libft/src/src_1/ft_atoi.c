/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 21:44:41 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:02:40 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

int	ft_atoi(const char *str)
{
	size_t	i;
	int		nbr;
	char	n;

	i = 0;
	nbr = 0;
	n = 1;
	while (str[i] == ' ' || str[i] == '\f' || str[i] == '\n'
		|| str[i] == '\r' || str[i] == '\t' || str[i] == '\v')
		i++;
	if (str[i] == '+' || str[i] == '-')
		n = 44 - str[i++];
	while (str[i] >= '0' && str[i] <= '9')
		nbr = 10 * nbr + str[i++] - 48;
	nbr *= n;
	return (nbr);
}
