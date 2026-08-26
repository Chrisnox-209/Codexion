/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 12:08:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/23 12:08:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

static void	wait_for_request(t_coder *coder, t_dongle *dongle,
		t_request *request)
{
	struct timespec	time;

	while (!simulation_stopped(coder->simulation)
		&& !request_ready(dongle, request))
	{
		if (dongle->owner_id == 0 && heap_peek(&dongle->queue) == request)
		{
			ms_to_timespec(dongle->available_at, &time);
			pthread_cond_timedwait(&dongle->condition,
				&dongle->mutex, &time);
		}
		else
			pthread_cond_wait(&dongle->condition, &dongle->mutex);
	}
}

static int	finish_request(t_coder *coder, t_dongle *dongle,
		t_request *request)
{
	int	acquired;

	acquired = !simulation_stopped(coder->simulation);
	if (acquired)
	{
		heap_pop(&dongle->queue);
		dongle->owner_id = coder->id;
	}
	else
		heap_remove(&dongle->queue, request);
	pthread_cond_broadcast(&dongle->condition);
	pthread_mutex_unlock(&dongle->mutex);
	if (acquired)
		log_state(coder, "has taken a dongle");
	return (acquired);
}

int	take_one_dongle(t_coder *coder, t_dongle *dongle)
{
	t_request	request;

	request.coder = coder;
	request.deadline = coder_deadline(coder);
	pthread_mutex_lock(&dongle->mutex);
	request.order = dongle->next_order++;
	if (heap_push(&dongle->queue, &request))
	{
		pthread_mutex_unlock(&dongle->mutex);
		return (0);
	}
	wait_for_request(coder, dongle, &request);
	return (finish_request(coder, dongle, &request));
}
