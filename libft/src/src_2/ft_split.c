/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jharrach <jharrach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 18:54:32 by jharrach          #+#    #+#             */
/*   Updated: 2023/02/10 20:04:09 by jharrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

static size_t	ft_word_len(char const *s, char c)
{
	size_t	len;

	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

static size_t	ft_count_words(char const *s, char c)
{
	size_t	j;

	j = 0;
	while (*s)
	{
		if (*s != c)
		{
			j++;
			s += ft_word_len(s, c);
		}
		else
			s++;
	}
	return (j);
}

static char	**ft_free_words(char **r, size_t j)
{
	size_t	i;

	i = 0;
	while (i < j && r[i])
		free(r[i++]);
	free(r);
	return (0);
}

static char	**ft_copy_words(char **r, const char *s, char c)
{
	size_t	j;
	size_t	len;

	j = 0;
	while (*s)
	{
		if (*s != c)
		{
			len = ft_word_len(s, c);
			r[j] = (char *)malloc(sizeof(char) * (len + 1));
			if (!r[j])
				return (ft_free_words(r, j));
			ft_strlcpy(r[j++], s, len + 1);
			s += len;
		}
		else
			s++;
	}
	r[j] = NULL;
	return (r);
}

char	**ft_split(char const *s, char c)
{
	char	**r;

	if (!s)
		return (0);
	r = (char **)malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!r)
		return (0);
	r = ft_copy_words(r, s, c);
	if (!r)
		return (0);
	return (r);
}
