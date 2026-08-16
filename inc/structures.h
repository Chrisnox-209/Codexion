/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 10:15:00 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/16 10:15:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

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
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		monitor;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	int				stopped;
	long			start_ms;
};

#endif
