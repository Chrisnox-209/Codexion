/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/19 10:25:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/19 10:25:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

# include "structures.h"

int			heap_init(t_heap *heap, int capacity, int is_edf);
void		heap_destroy(t_heap *heap);
int			heap_push(t_heap *heap, t_request *request);
t_request	*heap_peek(t_heap *heap);
t_request	*heap_pop(t_heap *heap);
void		heap_remove(t_heap *heap, t_request *request);
int			request_before(t_heap *heap, t_request *a, t_request *b);

#endif
