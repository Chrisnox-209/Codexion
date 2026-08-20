/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 11:40:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/18 11:40:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

void	destroy_dongles(t_simulation *simulation, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		heap_destroy(&simulation->dongles[i].queue);
		pthread_cond_destroy(&simulation->dongles[i].condition);
		pthread_mutex_destroy(&simulation->dongles[i].mutex);
		i++;
	}
}

void	clear_initialization(t_simulation *simulation, int dongles_ready)
{
	if (dongles_ready)
		destroy_dongles(simulation, simulation->config.nb_coders);
	pthread_mutex_destroy(&simulation->print_mutex);
	pthread_mutex_destroy(&simulation->stop_mutex);
	free(simulation->coders);
	free(simulation->dongles);
}

void	destroy_simulation(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->config.nb_coders)
	{
		pthread_mutex_destroy(&simulation->coders[i].state_mutex);
		i++;
	}
	destroy_dongles(simulation, simulation->config.nb_coders);
	pthread_mutex_destroy(&simulation->print_mutex);
	pthread_mutex_destroy(&simulation->stop_mutex);
	free(simulation->coders);
	free(simulation->dongles);
}
