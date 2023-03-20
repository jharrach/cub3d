/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/26 14:48:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:05:25 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static int	ft_print_arg(t_placeholder ph, va_list args)
{
	if (ph.type == 'c')
		return (ft_printf_c(va_arg(args, int), ph));
	if (ph.type == 's')
		return (ft_printf_s(va_arg(args, char *), ph));
	if (ph.type == 'p')
		return (ft_printf_p((uintptr_t)va_arg(args, void *), ph));
	if (ph.type == 'd' || ph.type == 'i')
		return (ft_printf_d(va_arg(args, int), ph));
	if (ph.type == 'u')
		return (ft_printf_u(va_arg(args, unsigned int), ph));
	if (ph.type == 'X' || ph.type == 'x')
		return (ft_printf_x(va_arg(args, unsigned int), ph));
	if (ph.type == '%')
		return (ft_printf_c('%', ph));
	return (0);
}

static int	ft_formatlen(const char *format)
{
	int	len;

	len = 0;
	while (format[len] && format[len] != '%')
		len++;
	return (len);
}

int	ft_printf(const char *format, ...)
{
	int				len;
	va_list			args1;
	t_placeholder	ph;

	va_start(args1, format);
	len = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format = ft_getph(&ph, ++format);
			len += ft_print_arg(ph, args1);
		}
		else
		{
			len += write(1, format, ft_formatlen(format));
			format += ft_formatlen(format);
		}
	}
	va_end(args1);
	return (len);
}
