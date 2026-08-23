/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 11:20:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/15 11:20:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

int	simulation_stopped(t_simulation *simulation)
{
	int	stopped;

	pthread_mutex_lock(&simulation->stop_mutex);
	stopped = simulation->stopped;
	pthread_mutex_unlock(&simulation->stop_mutex);
	return (stopped);
}

void	stop_simulation(t_simulation *simulation)
{
	pthread_mutex_lock(&simulation->stop_mutex);
	simulation->stopped = 1;
	pthread_mutex_unlock(&simulation->stop_mutex);
	wake_all_dongles(simulation);
}

void	wake_all_dongles(t_simulation *simulation)
{
	int	index;

	index = 0;
	while (index < simulation->config.nb_coders)
	{
		pthread_mutex_lock(&simulation->dongles[index].mutex);
		pthread_cond_broadcast(&simulation->dongles[index].condition);
		pthread_mutex_unlock(&simulation->dongles[index].mutex);
		index++;
	}
}

void	log_state(t_coder *coder, char *message)
{
	t_simulation	*simulation;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->print_mutex);
	pthread_mutex_lock(&simulation->stop_mutex);
	if (!simulation->stopped)
		printf("%ld %d %s\n", current_time_ms() - simulation->start_ms,
			coder->id, message);
	pthread_mutex_unlock(&simulation->stop_mutex);
	pthread_mutex_unlock(&simulation->print_mutex);
}

void	log_burnout(t_coder *coder)
{
	t_simulation	*simulation;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->print_mutex);
	pthread_mutex_lock(&simulation->stop_mutex);
	if (!simulation->stopped)
	{
		simulation->stopped = 1;
		printf("%ld %d burned out\n", current_time_ms()
			- simulation->start_ms, coder->id);
	}
	pthread_mutex_unlock(&simulation->stop_mutex);
	pthread_mutex_unlock(&simulation->print_mutex);
	wake_all_dongles(simulation);
}
