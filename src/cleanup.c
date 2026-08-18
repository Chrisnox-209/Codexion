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

void	clear_initialization(t_simulation *simulation, int dongles_ready)
{
	int	i;

	i = 0;
	while (dongles_ready && i < simulation->config.nb_coders)
	{
		pthread_mutex_destroy(&simulation->dongles[i].mutex);
		i++;
	}
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
		pthread_mutex_destroy(&simulation->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&simulation->print_mutex);
	pthread_mutex_destroy(&simulation->stop_mutex);
	free(simulation->coders);
	free(simulation->dongles);
}
