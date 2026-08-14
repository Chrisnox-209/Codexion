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

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	int				compile_count;
	t_simulation	*simulation;
}						t_coder;

struct s_simulation
{
	t_config	config;
	t_coder		*coders;
	long		start_ms;
};

int		init_simulation(t_simulation *simulation, t_config *config);
int		run_simulation(t_simulation *simulation);
void	destroy_simulation(t_simulation *simulation);
void	*coder_routine(void *data);
long	current_time_ms(void);
void	sleep_ms(long duration);

#endif
