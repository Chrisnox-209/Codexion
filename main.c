/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cpietrza <cpietrza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 16:24:18 by cpietrza          #+#    #+#             */
/*   Updated: 2026/09/15 16:27:10 by cpietrza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

int	main(int argc, char **argv)
{
	t_config	config;
	t_simulation	simulation;

	if (argc != 9)
	{
		write(2, "[ERROR]: The number of arguments is incorrect.\n", 47);
		return (1);
	}
	if (parse_arguments(argc, argv, &config) != 0)
	{
		write(2, "[ERROR]: Invalid arguments.\n", 28);
		return (1);
	}
	if (init_simulation(&simulation, &config) != 0)
		return (1);
	if (run_simulation(&simulation) != 0)
	{
		destroy_simulation(&simulation);
		return (1);
	}
	destroy_simulation(&simulation);
	return (0);
}
