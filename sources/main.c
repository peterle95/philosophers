/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:36:15 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 23:48:05 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	run_simulation(int argc, char **argv)
{
	t_data	data;

	if (init_data(&data, argc, argv) != 0)
		return (1);
	if (create_and_run_threads(&data) != 0)
	{
		cleanup(&data);
		return (1);
	}
	cleanup(&data);
	return (0);
}

int	main(int argc, char **argv)
{
	if (validate_input(argc, argv) != 0)
		return (EXIT_FAILURE);
	if (run_simulation(argc, argv) != 0)
		return (EXIT_FAILURE);
	return (0);
}
