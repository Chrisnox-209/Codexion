/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 14:35:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/14 14:35:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>

void	*coder_routine(void *data)
{
	t_coder		*coder;
	t_config	*config;

	coder = (t_coder *)data;
	config = &coder->simulation->config;
	printf("%ld %d is compiling\n", current_time_ms()
		- coder->simulation->start_ms, coder->id);
	sleep_ms(config->t_compile);
	printf("%ld %d is debugging\n", current_time_ms()
		- coder->simulation->start_ms, coder->id);
	sleep_ms(config->t_debug);
	printf("%ld %d is refactoring\n", current_time_ms()
		- coder->simulation->start_ms, coder->id);
	sleep_ms(config->t_refactor);
	coder->compile_count++;
	return (NULL);
}

int	run_simulation(t_simulation *simulation)
{
	int	i;

	simulation->start_ms = current_time_ms();
	i = 0;
	while (i < simulation->config.nb_coders)
	{
		if (pthread_create(&simulation->coders[i].thread, NULL,
				coder_routine, &simulation->coders[i]) != 0)
		{
			while (--i >= 0)
				pthread_join(simulation->coders[i].thread, NULL);
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < simulation->config.nb_coders)
	{
		pthread_join(simulation->coders[i].thread, NULL);
		i++;
	}
	return (0);
}
