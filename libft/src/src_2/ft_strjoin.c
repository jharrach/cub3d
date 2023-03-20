/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 18:04:38 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:04:14 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*r;
	size_t	l1;
	size_t	l2;

	if (!s1 || !s2)
		return (0);
	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	r = (char *)malloc(sizeof(char) * (l1 + l2 + 1));
	if (!r)
		return (0);
	ft_strlcpy(r, s1, l1 + 1);
	ft_strlcat(r, s2, l1 + l2 + 1);
	return (r);
}
