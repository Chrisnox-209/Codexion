/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 10:16:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/21 10:16:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

static long	coder_deadline(t_coder *coder)
{
	long	deadline;

	pthread_mutex_lock(&coder->state_mutex);
	deadline = coder->last_compile_ms
		+ coder->simulation->config.t_burnout;
	pthread_mutex_unlock(&coder->state_mutex);
	return (deadline);
}

static int	request_ready(t_dongle *dongle, t_request *request)
{
	if (dongle->owner_id != 0)
		return (0);
	if (heap_peek(&dongle->queue) != request)
		return (0);
	return (current_time_ms() >= dongle->available_at);
}

int	take_one_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	request;
	int			acquired;

	acquired = 0;
	request.coder = coder;
	request.deadline = coder_deadline(coder);
	pthread_mutex_lock(&dongle->mutex);
	request.order = dongle->next_order++;
	heap_push(&dongle->queue, &request);
	while (!simulation_stopped(coder->simulation)
		&& !request_ready(dongle, &request))
	{
		pthread_mutex_unlock(&dongle->mutex);
		usleep(500);
		pthread_mutex_lock(&dongle->mutex);
	}
	if (simulation_stopped(coder->simulation))
		heap_remove(&dongle->queue, &request);
	else
	{
		heap_pop(&dongle->queue);
		dongle->owner_id = coder->id;
		acquired = 1;
	}
	pthread_mutex_unlock(&dongle->mutex);
	if (!acquired)
		return (0);
	log_state(coder, "has taken a dongle");
	return (1);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->owner_id = 0;
	dongle->available_at = current_time_ms()
		+ coder->simulation->config.cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}

int	take_dongles(t_coder *coder)
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
	if (!take_one_dongle(coder, first))
		return (0);
	if (take_one_dongle(coder, second))
		return (1);
	release_dongle(coder, first);
	return (0);
}
