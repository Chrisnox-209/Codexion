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
	if (!take_dongles(coder))
		return (0);
	pthread_mutex_lock(&coder->state_mutex);
	coder->last_compile_ms = current_time_ms();
	pthread_mutex_unlock(&coder->state_mutex);
	log_state(coder, "is compiling");
	simulation_sleep(coder->simulation,
		coder->simulation->config.t_compile);
	pthread_mutex_lock(&coder->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&coder->state_mutex);
	release_dongle(coder, coder->left_dongle);
	release_dongle(coder, coder->right_dongle);
	return (1);
}

static void	one_coder(t_coder *coder)
{
	if (!take_one_dongle(coder, coder->left_dongle))
		return ;
	while (!simulation_stopped(coder->simulation))
		usleep(500);
	release_dongle(coder, coder->left_dongle);
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
