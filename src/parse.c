/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 16:24:22 by cpietrza          #+#    #+#             */
/*   Updated: 2026/09/15 16:28:38 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	parse_arguments(int argc, char **argv, t_config *config)
{
	int	i;

	if (argc != 9)
		return (1);
	i = 0;
	while (++i <= 7)
		if (!is_valid_number(argv[i]))
			return (1);
	config->nb_coders = atoi(argv[1]);
	config->t_burnout = atoi(argv[2]);
	config->t_compile = atoi(argv[3]);
	config->t_debug = atoi(argv[4]);
	config->t_refactor = atoi(argv[5]);
	config->nb_compiles = atoi(argv[6]);
	config->cooldown = atoi(argv[7]);
	if (strcmp(argv[8], "fifo") == 0)
		config->is_edf = 0;
	else if (strcmp(argv[8], "edf") == 0)
		config->is_edf = 1;
	else
		return (1);
	if (config->nb_coders <= 0)
		return (1);
	return (0);
}
