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

int		init_simulation(t_simulation *simulation, t_config *config);
int		run_simulation(t_simulation *simulation);
void	destroy_simulation(t_simulation *simulation);
void	*coder_routine(void *data);
void	*monitor_routine(void *data);
int		simulation_stopped(t_simulation *simulation);
void	stop_simulation(t_simulation *simulation);
void	log_state(t_coder *coder, char *message);
void	log_burnout(t_coder *coder);
long	current_time_ms(void);
void	sleep_ms(long duration);
void	simulation_sleep(t_simulation *simulation, long duration);

#endif
