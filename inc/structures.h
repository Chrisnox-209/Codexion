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
# include <time.h>

typedef struct s_simulation	t_simulation;
typedef struct s_coder		t_coder;

typedef struct s_request
{
	t_coder	*coder;
	long	order;
	long	deadline;
	long	wait_started_ms;
}						t_request;

typedef struct s_heap
{
	t_request	**items;
	int			size;
	int			capacity;
	int			is_edf;
}						t_heap;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	pthread_cond_t	condition;
	t_heap			queue;
	int				owner_id;
	long			available_at;
	long			next_order;
}						t_dongle;

struct s_coder
{
	int				id;
	pthread_t		thread;
	int				compile_count;
	long			last_compile_ms;
	pthread_mutex_t	state_mutex;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_simulation	*simulation;
};

struct s_simulation
{
	t_config		config;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_t		monitor;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	pair_mutex;
	pthread_cond_t	pair_condition;
	t_heap			pair_queue;
	long			next_pair_order;
	long			next_pair_at;
	int				stopped;
	long			start_ms;
};

#endif
