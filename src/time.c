/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/14 11:08:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/14 11:08:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <sys/time.h>
#include <unistd.h>

long	current_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000L + time.tv_usec / 1000L);
}

int	simulation_sleep(t_simulation *simulation, long duration)
{
	long	end;

	end = current_time_ms() + duration;
	while (current_time_ms() < end && !simulation_stopped(simulation))
		usleep(500);
	return (!simulation_stopped(simulation));
}

void	ms_to_timespec(long milliseconds, struct timespec *time)
{
	time->tv_sec = milliseconds / 1000;
	time->tv_nsec = (milliseconds % 1000) * 1000000;
}
