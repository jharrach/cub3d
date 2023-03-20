/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 18:28:53 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:04:19 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static char	ft_cins(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*r;
	size_t	l;

	if (!s1 || !set)
		return (0);
	l = ft_strlen(s1);
	while (l && ft_cins(s1[l - 1], set))
		l--;
	while (*s1 && l && ft_cins(*s1, set))
	{	
		l--;
		s1++;
	}
	r = (char *)malloc(sizeof(char) * (l + 1));
	if (!r)
		return (0);
	ft_strlcpy(r, s1, l + 1);
	return (r);
}
