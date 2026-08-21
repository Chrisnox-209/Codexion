/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 16:24:05 by cpietrza          #+#    #+#             */
/*   Updated: 2026/09/15 16:26:09 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include "structures.h"
# include "heap.h"

int		init_simulation(t_simulation *simulation, t_config *config);
int		init_dongles(t_simulation *simulation);
int		run_simulation(t_simulation *simulation);
void	destroy_simulation(t_simulation *simulation);
void	clear_initialization(t_simulation *simulation, int dongles_ready);
void	destroy_dongles(t_simulation *simulation, int count);
void	*coder_routine(void *data);
void	*monitor_routine(void *data);
int		simulation_stopped(t_simulation *simulation);
void	stop_simulation(t_simulation *simulation);
void	log_state(t_coder *coder, char *message);
void	log_burnout(t_coder *coder);
int		take_dongles(t_coder *coder);
int		take_one_dongle(t_coder *coder, t_dongle *dongle);
void	release_dongle(t_coder *coder, t_dongle *dongle);
long	current_time_ms(void);
void	sleep_ms(long duration);
void	simulation_sleep(t_simulation *simulation, long duration);

#endif
