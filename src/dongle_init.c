/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 10:42:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/20 10:42:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_one_dongle(t_simulation *simulation, int index)
{
	t_dongle	*dongle;

	dongle = &simulation->dongles[index];
	dongle->id = index;
	if (pthread_mutex_init(&dongle->mutex, NULL) != 0)
		return (1);
	if (pthread_cond_init(&dongle->condition, NULL) != 0)
	{
		pthread_mutex_destroy(&dongle->mutex);
		return (1);
	}
	if (heap_init(&dongle->queue, simulation->config.nb_coders, 0) != 0)
	{
		pthread_cond_destroy(&dongle->condition);
		pthread_mutex_destroy(&dongle->mutex);
		return (1);
	}
	return (0);
}

int	init_dongles(t_simulation *simulation)
{
	int	index;

	index = 0;
	while (index < simulation->config.nb_coders)
	{
		if (init_one_dongle(simulation, index))
		{
			destroy_dongles(simulation, index);
			return (1);
		}
		index++;
	}
	return (0);
}
