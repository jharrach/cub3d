/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 19:45:33 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:03:28 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	dsize;
	char	*d;

	src_len = ft_strlen(src);
	d = dst;
	dsize = dstsize;
	while (dsize-- && *dst)
		dst++;
	dst_len = dst - d;
	if (dst_len >= dstsize)
		return (src_len + dstsize);
	dstsize -= dst_len;
	while (--dstsize && *src)
	{	
		*dst = *src;
		src++;
		dst++;
	}
	*dst = 0;
	return (src_len + dst_len);
}
