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

# include "parsing.h"
# include <pthread.h>

typedef struct s_simulation	t_simulation;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
}						t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	int				compile_count;
	long			last_compile_ms;
	pthread_mutex_t	state_mutex;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_simulation	*simulation;
}						t_coder;

struct s_simulation
{
	t_config	config;
	t_coder		*coders;
	t_dongle	*dongles;
	pthread_t	monitor;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	int			stopped;
	long		start_ms;
};

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
