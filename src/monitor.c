/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/15 15:05:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/15 15:05:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

static int	check_coders(t_simulation *simulation)
{
	int		i;
	int		completed;
	long	last_compile;

	i = 0;
	completed = 1;
	while (i < simulation->config.nb_coders)
	{
		pthread_mutex_lock(&simulation->coders[i].state_mutex);
		last_compile = simulation->coders[i].last_compile_ms;
		if (simulation->coders[i].compile_count
			< simulation->config.nb_compiles)
			completed = 0;
		pthread_mutex_unlock(&simulation->coders[i].state_mutex);
		if (current_time_ms() - last_compile
			>= simulation->config.t_burnout)
		{
			log_burnout(&simulation->coders[i]);
			return (1);
		}
		i++;
	}
	if (completed)
	{
		stop_simulation(simulation);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *data)
{
	t_simulation	*simulation;

	simulation = (t_simulation *)data;
	while (!simulation_stopped(simulation))
	{
		if (check_coders(simulation))
			break ;
		usleep(500);
	}
	return (NULL);
}
