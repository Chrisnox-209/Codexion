/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 12:00:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/09/16 12:00:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	pair_available(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;
	int			available;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first->id > second->id)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	available = first->owner_id == 0 && second->owner_id == 0
		&& current_time_ms() >= first->available_at
		&& current_time_ms() >= second->available_at;
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
	return (available);
}

static int	request_selected(t_simulation *simulation, t_request *request)
{
	int	index;

	if (!pair_available(request->coder))
		return (0);
	index = 0;
	while (index < simulation->pair_queue.size)
	{
		if (request_before(&simulation->pair_queue,
				simulation->pair_queue.items[index], request)
			&& pair_available(simulation->pair_queue.items[index]->coder))
			return (0);
		index++;
	}
	return (1);
}

static void	own_pair(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first->id > second->id)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	pthread_mutex_lock(&first->mutex);
	pthread_mutex_lock(&second->mutex);
	first->owner_id = coder->id;
	second->owner_id = coder->id;
	pthread_mutex_unlock(&second->mutex);
	pthread_mutex_unlock(&first->mutex);
}

static int	wait_for_pair(t_simulation *simulation, t_request *request)
{
	struct timespec	time;

	while (!simulation_stopped(simulation)
		&& !request_selected(simulation, request))
	{
		ms_to_timespec(current_time_ms() + 1, &time);
		pthread_cond_timedwait(&simulation->pair_condition,
			&simulation->pair_mutex, &time);
	}
	heap_remove(&simulation->pair_queue, request);
	if (simulation_stopped(simulation))
		return (0);
	own_pair(request->coder);
	return (1);
}

int	take_dongles(t_coder *coder)
{
	t_simulation	*simulation;
	t_request		request;
	int				acquired;

	simulation = coder->simulation;
	request.coder = coder;
	request.deadline = coder_deadline(coder);
	pthread_mutex_lock(&simulation->pair_mutex);
	request.order = simulation->next_pair_order++;
	if (heap_push(&simulation->pair_queue, &request))
		return (pthread_mutex_unlock(&simulation->pair_mutex), 0);
	acquired = wait_for_pair(simulation, &request);
	pthread_cond_broadcast(&simulation->pair_condition);
	pthread_mutex_unlock(&simulation->pair_mutex);
	if (!acquired)
		return (0);
	log_state(coder, "has taken a dongle");
	log_state(coder, "has taken a dongle");
	return (1);
}
