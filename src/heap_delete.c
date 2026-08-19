/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_delete.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 16:05:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/19 16:05:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

static int	request_index(t_heap *heap, t_request *request)
{
	int	index;

	index = 0;
	while (index < heap->size)
	{
		if (heap->items[index] == request)
			return (index);
		index++;
	}
	return (-1);
}

static void	swap_requests(t_request **first, t_request **second)
{
	t_request	*temporary;

	temporary = *first;
	*first = *second;
	*second = temporary;
}

static int	move_up(t_heap *heap, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_before(heap, heap->items[index], heap->items[parent]))
			break ;
		swap_requests(&heap->items[index], &heap->items[parent]);
		index = parent;
	}
	return (index);
}

static int	best_child(t_heap *heap, int index)
{
	int	left;
	int	right;

	left = index * 2 + 1;
	right = left + 1;
	if (right < heap->size && request_before(heap,
			heap->items[right], heap->items[left]))
		return (right);
	return (left);
}

void	heap_remove(t_heap *heap, t_request *request)
{
	int	index;
	int	child;

	index = request_index(heap, request);
	if (index < 0)
		return ;
	heap->size--;
	if (index == heap->size)
		return ;
	heap->items[index] = heap->items[heap->size];
	index = move_up(heap, index);
	while (index * 2 + 1 < heap->size)
	{
		child = best_child(heap, index);
		if (!request_before(heap, heap->items[child], heap->items[index]))
			break ;
		swap_requests(&heap->items[index], &heap->items[child]);
		index = child;
	}
}
