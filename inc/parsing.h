/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 16:24:13 by cpietrza          #+#    #+#             */
/*   Updated: 2026/09/15 16:25:49 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <stdlib.h>
# include <string.h>

typedef struct s_config
{
	int	nb_coders;
	int	t_burnout;
	int	t_compile;
	int	t_debug;
	int	t_refactor;
	int	nb_compiles;
	int	cooldown;
	int	is_edf;
}		t_config;

int		parse_arguments(int argc, char **argv, t_config *config);
int		is_valid_number(char *str);

#endif