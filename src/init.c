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
	if (simulation->coders == NULL)
		return (1);
	memset(simulation->coders, 0, sizeof(t_coder) * config->nb_coders);
	i = 0;
	while (i < config->nb_coders)
	{
		simulation->coders[i].id = i + 1;
		simulation->coders[i].simulation = simulation;
		i++;
	}
	return (0);
}

void	destroy_simulation(t_simulation *simulation)
{
	free(simulation->coders);
}
