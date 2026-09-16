/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 10:16:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/23 14:20:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	t_simulation	*simulation;

	simulation = coder->simulation;
	pthread_mutex_lock(&dongle->mutex);
	dongle->owner_id = 0;
	dongle->available_at = current_time_ms()
		+ simulation->config.cooldown;
	pthread_cond_broadcast(&dongle->condition);
	pthread_mutex_unlock(&dongle->mutex);
	pthread_mutex_lock(&simulation->pair_mutex);
	pthread_cond_broadcast(&simulation->pair_condition);
	pthread_mutex_unlock(&simulation->pair_mutex);
}
