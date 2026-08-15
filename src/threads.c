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

static void	lock_dongles(t_coder *coder)
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
	log_state(coder, "has taken a dongle");
	pthread_mutex_lock(&second->mutex);
	log_state(coder, "has taken a dongle");
}

static void	unlock_dongles(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left_dongle->mutex);
	pthread_mutex_unlock(&coder->right_dongle->mutex);
}

static void	compile_code(t_coder *coder)
{
	lock_dongles(coder);
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_ms = current_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	log_state(coder, "is compiling");
	simulation_sleep(coder->simulation,
		coder->simulation->config.t_compile);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
	unlock_dongles(coder);
}

static void	one_coder(t_coder *coder)
{
	pthread_mutex_lock(&coder->left_dongle->mutex);
	log_state(coder, "has taken a dongle");
	while (!simulation_stopped(coder->simulation))
		usleep(500);
	pthread_mutex_unlock(&coder->left_dongle->mutex);
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
		compile_code(coder);
		log_state(coder, "is debugging");
		simulation_sleep(coder->simulation,
			coder->simulation->config.t_debug);
		log_state(coder, "is refactoring");
		simulation_sleep(coder->simulation,
			coder->simulation->config.t_refactor);
	}
	return (NULL);
}

int	run_simulation(t_simulation *simulation)
{
	int	i;

	simulation->start_ms = current_time_ms();
	i = 0;
	while (i < simulation->config.nb_coders)
	{
		simulation->coders[i].last_compile_ms = simulation->start_ms;
		if (pthread_create(&simulation->coders[i].thread, NULL,
				coder_routine, &simulation->coders[i]) != 0)
			break ;
		i++;
	}
	if (i != simulation->config.nb_coders
		|| pthread_create(&simulation->monitor, NULL,
			monitor_routine, simulation) != 0)
		stop_simulation(simulation);
	else
		pthread_join(simulation->monitor, NULL);
	while (--i >= 0)
		pthread_join(simulation->coders[i].thread, NULL);
	return (0);
}
