/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 14:35:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/15 16:42:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

static int	compile_code(t_coder *coder)
{
	int	completed;

	if (!take_dongles(coder))
		return (0);
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_ms = current_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	log_state(coder, "is compiling");
	completed = simulation_sleep(coder->simulation,
			coder->simulation->config.t_compile);
	if (completed)
	{
		pthread_mutex_lock(&coder->state_mutex);
		coder->compile_count++;
		pthread_mutex_unlock(&coder->state_mutex);
	}
	release_dongle(coder, coder->left_dongle);
	release_dongle(coder, coder->right_dongle);
	return (completed);
}

static void	one_coder(t_coder *coder)
{
	if (!take_one_dongle(coder, coder->left_dongle))
		return ;
	while (!simulation_stopped(coder->simulation))
		usleep(500);
	release_dongle(coder, coder->left_dongle);
}

static void	debug_and_refactor(t_coder *coder)
{
	log_state(coder, "is debugging");
	simulation_sleep(coder->simulation,
		coder->simulation->config.t_debug);
	log_state(coder, "is refactoring");
	simulation_sleep(coder->simulation,
		coder->simulation->config.t_refactor);
}

void	*coder_routine(void *data)
{
	t_coder	*coder;

	coder = (t_coder *)data;
	if (coder->left_dongle == coder->right_dongle)
	{
		one_coder(coder);
		return (NULL);
	}
	if (coder->id % 2 == 0)
		simulation_sleep(coder->simulation,
			coder->simulation->config.t_compile / 2);
	while (!simulation_stopped(coder->simulation))
	{
		if (!compile_code(coder))
			break ;
		debug_and_refactor(coder);
	}
	return (NULL);
}
