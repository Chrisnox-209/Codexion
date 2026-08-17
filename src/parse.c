/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 16:24:22 by cpietrza          #+#    #+#             */
/*   Updated: 2026/08/17 14:10:00 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"
#include <limits.h>
#include <string.h>

static int	parse_number(char *text, int *number)
{
	long	value;
	int		digit;

	value = 0;
	if (*text == '+')
		text++;
	if (*text == '\0')
		return (1);
	while (*text)
	{
		if (*text < '0' || *text > '9')
			return (1);
		digit = *text - '0';
		if (value > (INT_MAX - digit) / 10)
			return (1);
		value = value * 10 + digit;
		text++;
	}
	*number = (int)value;
	return (0);
}

static int	parse_numbers(char **argv, t_config *config)
{
	if (parse_number(argv[1], &config->nb_coders)
		|| parse_number(argv[2], &config->t_burnout)
		|| parse_number(argv[3], &config->t_compile)
		|| parse_number(argv[4], &config->t_debug)
		|| parse_number(argv[5], &config->t_refactor)
		|| parse_number(argv[6], &config->nb_compiles)
		|| parse_number(argv[7], &config->cooldown))
		return (1);
	return (0);
}

static int	parse_scheduler(char *text, t_config *config)
{
	if (strcmp(text, "fifo") == 0)
		config->is_edf = 0;
	else if (strcmp(text, "edf") == 0)
		config->is_edf = 1;
	else
		return (1);
	return (0);
}

int	parse_arguments(int argc, char **argv, t_config *config)
{
	if (argc != 9)
		return (1);
	if (parse_numbers(argv, config))
		return (1);
	if (parse_scheduler(argv[8], config))
		return (1);
	if (config->nb_coders == 0)
		return (1);
	return (0);
}
