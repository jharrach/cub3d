/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_placeholder.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 19:09:44 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:05:06 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static char	ft_istype(const char c)
{
	int			i;
	const char	types[] = "cspdiuxX%";

	i = 0;
	while (types[i])
	{
		if (c == types[i])
			return (1);
		i++;
	}
	return (0);
}

static const char	*ft_formattoi(int *num, const char *format)
{
	while (ft_isdigit(*format))
		*num = *num * 10 + *format++ - '0';
	return (format);
}

static const char	*ft_getflag(int *flags, const char *format)
{
	int			i;
	const char	flag[] = "-0# +";

	i = 0;
	while (flag[i])
	{
		if (flag[i] == *format)
		{
			*flags |= 1 << i;
			format++;
			i = 0;
		}
		else
			i++;
	}
	return (format);
}

const char	*ft_getph(t_placeholder *ph, const char *format)
{
	ph->type = 0;
	ph->width = 0;
	ph->precision = 0;
	ph->flags = 0;
	format = ft_getflag(&(ph->flags), format);
	format = ft_formattoi(&(ph->width), format);
	if (*format == '.')
	{
		ph->flags += F_DOT;
		format = ft_formattoi(&(ph->precision), ++format);
	}
	if (ft_istype(*format))
		ph->type = *format++;
	return (format);
}
