/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 15:05:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/25 14:12:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

static int	coder_completed(t_coder *coder)
{
	int	completed;

	pthread_mutex_lock(&coder->state_mutex);
	completed = coder->compile_count
		>= coder->simulation->config.nb_compiles;
	pthread_mutex_unlock(&coder->state_mutex);
	return (completed);
}

static int	all_coders_completed(t_simulation *simulation)
{
	int	index;

	index = 0;
	while (index < simulation->config.nb_coders)
	{
		if (!coder_completed(&simulation->coders[index]))
			return (0);
		index++;
	}
	return (1);
}

static int	coder_burned_out(t_coder *coder, long now)
{
	long	last_compile;

	pthread_mutex_lock(&coder->state_mutex);
	last_compile = coder->last_compile_ms;
	pthread_mutex_unlock(&coder->state_mutex);
	return (now - last_compile >= coder->simulation->config.t_burnout);
}

static t_coder	*find_burned_out(t_simulation *simulation)
{
	int		index;
	long	now;

	index = 0;
	now = current_time_ms();
	while (index < simulation->config.nb_coders)
	{
		if (coder_burned_out(&simulation->coders[index], now))
			return (&simulation->coders[index]);
		index++;
	}
	return (NULL);
}

void	*monitor_routine(void *data)
{
	t_simulation	*simulation;
	t_coder			*burned_out;

	simulation = (t_simulation *)data;
	while (!simulation_stopped(simulation))
	{
		if (all_coders_completed(simulation))
		{
			stop_simulation(simulation);
			return (NULL);
		}
		burned_out = find_burned_out(simulation);
		if (burned_out != NULL)
		{
			log_burnout(burned_out);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
