/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 10:12:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/14 10:12:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>
#include <string.h>

int	init_simulation(t_simulation *simulation, t_config *config)
{
	int	i;

	memset(simulation, 0, sizeof(*simulation));
	simulation->config = *config;
	simulation->coders = malloc(sizeof(t_coder) * config->nb_coders);
	simulation->dongles = malloc(sizeof(t_dongle) * config->nb_coders);
	if (simulation->coders == NULL || simulation->dongles == NULL)
		return (1);
	memset(simulation->coders, 0, sizeof(t_coder) * config->nb_coders);
	memset(simulation->dongles, 0, sizeof(t_dongle) * config->nb_coders);
	if (pthread_mutex_init(&simulation->print_mutex, NULL) != 0
		|| pthread_mutex_init(&simulation->stop_mutex, NULL) != 0)
		return (1);
	i = 0;
	while (i < config->nb_coders)
	{
		simulation->dongles[i].id = i;
		pthread_mutex_init(&simulation->dongles[i].mutex, NULL);
		pthread_mutex_init(&simulation->coders[i].state_mutex, NULL);
		simulation->coders[i].id = i + 1;
		simulation->coders[i].simulation = simulation;
		simulation->coders[i].left_dongle = &simulation->dongles[i];
		simulation->coders[i].right_dongle = &simulation->dongles[(i + 1)
			% config->nb_coders];
		i++;
	}
	return (0);
}

void	destroy_simulation(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.nb_coders)
	{
		pthread_mutex_destroy(&simulation->coders[i].state_mutex);
		pthread_mutex_destroy(&simulation->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&simulation->print_mutex);
	pthread_mutex_destroy(&simulation->stop_mutex);
	free(simulation->coders);
	free(simulation->dongles);
}
