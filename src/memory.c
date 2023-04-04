/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_allocation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rburgsta <rburgsta@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 12:42:01 by rburgsta          #+#    #+#             */
/*   Updated: 2023/04/04 12:42:01 by rburgsta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3d.h"

void *ft_alloc(t_data *data, size_t count, size_t size)
{
	t_list	*new;
	void	*content;

	content = ft_calloc(count, size);
	if (content == NULL)
		destroy_data(data, 1, "Failed to allocate memory!");
	new = ft_lstnew(content);
	if (new == NULL)
	{
		free(content);
		destroy_data(data, 1, "Failed to allocate new node!");
	}
	ft_lstadd_back(data->head, new);
	return (content);
}

void *ft_alloc_add(t_data *data, void *content)
{
	t_list	*new;

	if (content == NULL)
		return (NULL);
	new = ft_lstnew(content);
	if (new == NULL)
	{
		free(content);
		destroy_data(data, 1, "Failed to allocate new node!");
	}
	ft_lstadd_back(data->head, new);
	return (content);
}

void ft_free(t_data *data, void *content)
{
	t_list	*prev;
	t_list	*current;

	if (data->head == NULL)
		free (content);
	else
	{
		current = *data->head;
		while (current != NULL && current->content != content)
		{
			prev = current;
			current = current->next;
		}
		if (current == NULL)
			free(content);
		else
		{
			prev->next = current->next;
			ft_lstdelone(current, &free);
		}
	}
}
