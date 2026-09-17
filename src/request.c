/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/17 10:00:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/09/17 10:00:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	requests_conflict(t_request *a, t_request *b)
{
	return (a->coder->left_dongle == b->coder->left_dongle
		|| a->coder->left_dongle == b->coder->right_dongle
		|| a->coder->right_dongle == b->coder->left_dongle
		|| a->coder->right_dongle == b->coder->right_dongle);
}

int	request_aged(t_simulation *simulation, t_request *request)
{
	if (simulation->config.nb_coders % 2 != 0
		|| simulation->config.nb_coders > 32)
		return (0);
	return (current_time_ms() - request->wait_started_ms
		> simulation->config.cooldown);
}
