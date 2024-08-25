/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 23:50:04 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/25 22:28:15 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Checks if all philosophers have eaten the required number of meals
bool	all_philosophers_ate_enough(t_data *data)
{
	int		i;

	i = 0;
	if (data->meals_to_eat == -1)
		return (false);
	while (i < data->num_philosophers)
	{
		if (data->philosophers[i].times_eaten < data->meals_to_eat)
			return (false);
		i++;
	}
	return (true);
}

// Checks if any philosopher has died in the simulation
int	check_philosopher_deaths(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (is_philosopher_dead(&data->philosophers[i], data))
			return (1);
		i++;
	}
	return (0);
}

// Checks if all philosophers have eaten enough and stops the simulation if true
int	check_all_philosophers_ate_enough(t_data *data)
{
	if (all_philosophers_ate_enough(data))
	{
		set_simulation_stop(data);
		return (1);
	}
	return (0);
}
