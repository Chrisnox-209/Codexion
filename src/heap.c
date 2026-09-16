/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 11:00:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/19 11:00:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "heap.h"
#include <stdlib.h>

static void	swap_requests(t_request **first, t_request **second)
{
	t_request	*temporary;

	temporary = *first;
	*first = *second;
	*second = temporary;
}

int	request_before(t_heap *heap, t_request *a, t_request *b)
{
	if (heap->is_edf && a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	if (heap->is_edf && a->coder->id != b->coder->id)
		return (a->coder->id > b->coder->id);
	if (a->order != b->order)
		return (a->order < b->order);
	return (a->coder->id < b->coder->id);
}

int	heap_init(t_heap *heap, int capacity, int is_edf)
{
	heap->items = malloc(sizeof(t_request *) * capacity);
	if (heap->items == NULL)
		return (1);
	heap->size = 0;
	heap->capacity = capacity;
	heap->is_edf = is_edf;
	return (0);
}

void	heap_destroy(t_heap *heap)
{
	free(heap->items);
	heap->items = NULL;
	heap->size = 0;
}

int	heap_push(t_heap *heap, t_request *request)
{
	int	index;
	int	parent;

	if (heap->size == heap->capacity)
		return (1);
	index = heap->size++;
	heap->items[index] = request;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!request_before(heap, heap->items[index], heap->items[parent]))
			break ;
		swap_requests(&heap->items[index], &heap->items[parent]);
		index = parent;
	}
	return (0);
}
