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
	pthread_mutex_lock(&dongle->mutex);
	dongle->owner_id = 0;
	dongle->available_at = current_time_ms()
		+ coder->simulation->config.cooldown;
	pthread_cond_broadcast(&dongle->condition);
	pthread_mutex_unlock(&dongle->mutex);
}

int	take_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	first = coder->left_dongle;
	second = coder->right_dongle;
	if (first->id > second->id)
	{
		first = coder->right_dongle;
		second = coder->left_dongle;
	}
	if (!take_one_dongle(coder, first))
		return (0);
	if (take_one_dongle(coder, second))
		return (1);
	release_dongle(coder, first);
	return (0);
}
