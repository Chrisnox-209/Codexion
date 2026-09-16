/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 11:32:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/09/16 10:00:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_coders(t_simulation *simulation)
{
	int	created;

	created = 0;
	while (created < simulation->config.nb_coders)
	{
		if (pthread_create(&simulation->coders[created].thread, NULL,
				coder_routine, &simulation->coders[created]) != 0)
			break ;
		created++;
	}
	return (created);
}

static void	join_coders(t_simulation *simulation, int count)
{
	int	index;

	index = 0;
	while (index < count)
	{
		pthread_join(simulation->coders[index].thread, NULL);
		index++;
	}
}

static void	set_start_times(t_simulation *simulation)
{
	int	index;

	index = 0;
	while (index < simulation->config.nb_coders)
	{
		simulation->coders[index].last_compile_ms = simulation->start_ms;
		index++;
	}
}

int	run_simulation(t_simulation *simulation)
{
	int	created;

	simulation->start_ms = current_time_ms();
	set_start_times(simulation);
	if (pthread_create(&simulation->monitor, NULL,
			monitor_routine, simulation) != 0)
		return (1);
	created = create_coders(simulation);
	if (created != simulation->config.nb_coders)
	{
		stop_simulation(simulation);
		join_coders(simulation, created);
		pthread_join(simulation->monitor, NULL);
		return (1);
	}
	pthread_join(simulation->monitor, NULL);
	join_coders(simulation, created);
	return (0);
}
