/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_remove.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 14:30:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/19 14:30:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"

static void	swap_requests(t_request **first, t_request **second)
{
	t_request	*temporary;

	temporary = *first;
	*first = *second;
	*second = temporary;
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

static void	move_down(t_heap *heap, int index)
{
	int	child;

	while (index * 2 + 1 < heap->size)
	{
		child = best_child(heap, index);
		if (!request_before(heap, heap->items[child], heap->items[index]))
			break ;
		swap_requests(&heap->items[index], &heap->items[child]);
		index = child;
	}
}

t_request	*heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (heap->items[0]);
}

t_request	*heap_pop(t_heap *heap)
{
	t_request	*first;

	if (heap->size == 0)
		return (NULL);
	first = heap->items[0];
	heap->size--;
	if (heap->size > 0)
	{
		heap->items[0] = heap->items[heap->size];
		move_down(heap, 0);
	}
	return (first);
}
