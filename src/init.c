/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 10:12:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/18 15:18:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>
#include <string.h>

static int	allocate_arrays(t_simulation *simulation)
{
	int	size;

	size = simulation->config.nb_coders;
	simulation->coders = malloc(sizeof(t_coder) * size);
	simulation->dongles = malloc(sizeof(t_dongle) * size);
	if (simulation->coders == NULL || simulation->dongles == NULL)
	{
		free(simulation->coders);
		free(simulation->dongles);
		return (1);
	}
	memset(simulation->coders, 0, sizeof(t_coder) * size);
	memset(simulation->dongles, 0, sizeof(t_dongle) * size);
	return (0);
}

static int	init_main_mutexes(t_simulation *simulation)
{
	if (pthread_mutex_init(&simulation->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&simulation->stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&simulation->print_mutex);
		return (1);
	}
	return (0);
}

static int	init_dongles(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.nb_coders)
	{
		simulation->dongles[i].id = i;
		if (pthread_mutex_init(&simulation->dongles[i].mutex, NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&simulation->dongles[i].mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	init_coders(t_simulation *simulation)
{
	int	i;
	int	count;

	i = 0;
	count = simulation->config.nb_coders;
	while (i < count)
	{
		if (pthread_mutex_init(&simulation->coders[i].state_mutex, NULL))
			break ;
		simulation->coders[i].id = i + 1;
		simulation->coders[i].simulation = simulation;
		simulation->coders[i].left_dongle = &simulation->dongles[i];
		simulation->coders[i].right_dongle = &simulation->dongles[(i + 1)
			% count];
		i++;
	}
	if (i == count)
		return (0);
	while (--i >= 0)
		pthread_mutex_destroy(&simulation->coders[i].state_mutex);
	return (1);
}

int	init_simulation(t_simulation *simulation, t_config *config)
{
	memset(simulation, 0, sizeof(*simulation));
	simulation->config = *config;
	if (allocate_arrays(simulation))
		return (1);
	if (init_main_mutexes(simulation))
	{
		free(simulation->coders);
		free(simulation->dongles);
		return (1);
	}
	if (init_dongles(simulation))
	{
		clear_initialization(simulation, 0);
		return (1);
	}
	if (!init_coders(simulation))
		return (0);
	clear_initialization(simulation, 1);
	return (1);
}
