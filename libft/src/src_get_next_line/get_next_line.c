/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/12 17:52:25 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:05:01 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static char	*ft_error_free(char *line, char **old_line)
{
	if (line)
		free(line);
	if (*old_line)
	{
		free(*old_line);
		*old_line = 0;
	}
	return (0);
}

static char	*ft_get_old_line(char **old_line, int fd)
{
	char	*line;
	int		len;

	if (fd < 0 || fd > OPEN_MAX)
		return (0);
	if (*old_line)
	{
		len = ft_strlen(*old_line);
		line = ft_calloc(len + BUFFER_SIZE + 1, sizeof(char));
		if (!line)
			return (ft_error_free(line, old_line));
		ft_strlcpy(line, *old_line, len + 1);
		free(*old_line);
		*old_line = 0;
	}
	else
	{
		line = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
		if (!line)
			return (0);
	}
	return (line);
}

static char	*ft_split_line(char *line, char **old_line)
{
	char	*end;

	end = ft_strchr(line, '\n') + 1;
	if (*end)
	{
		*old_line = ft_strdup(end);
		if (!*old_line)
			return (ft_error_free(line, old_line));
		*end = 0;
	}
	return (line);
}

static char	*ft_get_line(char *line, int fd, char **old_line)
{
	ssize_t	read_return;
	size_t	len;
	char	*tmp;

	len = ft_strlen(line);
	read_return = 1;
	while (read_return)
	{
		if (ft_strchr(line, '\n'))
			return (ft_split_line(line, old_line));
		read_return = read(fd, line + len, BUFFER_SIZE);
		if (read_return < 0)
			return (ft_error_free(line, old_line));
		len += BUFFER_SIZE;
		tmp = ft_calloc(len + BUFFER_SIZE + 1, sizeof(char));
		if (!tmp)
			return (ft_error_free(line, old_line));
		ft_strlcpy(tmp, line, len + 1);
		free(line);
		line = tmp;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*old_lines[OPEN_MAX];
	char		*line;
	char		*tmp;

	line = ft_get_old_line(&old_lines[fd], fd);
	if (!line)
		return (0);
	line = ft_get_line(line, fd, &old_lines[fd]);
	if (!line)
		return (0);
	tmp = ft_strdup(line);
	free(line);
	line = tmp;
	if (!line || !line[0])
		return (ft_error_free(line, &old_lines[fd]));
	return (line);
}
