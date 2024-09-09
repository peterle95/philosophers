/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 23:50:04 by pmolzer           #+#    #+#             */
/*   Updated: 2024/09/10 00:14:30 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Checks if all philosophers have eaten the required number of meals
bool	all_philosophers_ate_enough(t_data *data)
{
	int		i;

	i = 0;
	if (data->meals_to_eat == -1) // If meals_to_eat is not set, return false
		return (false);

	pthread_mutex_lock(&data->stop_mutex); // Lock before accessing shared data
	while (i < data->num_philosophers)
	{
		if (data->philosophers[i].times_eaten < data->meals_to_eat)
		{
			pthread_mutex_unlock(&data->stop_mutex); // Unlock before returning
			return (false);
		}
		i++;
	}
	pthread_mutex_unlock(&data->stop_mutex); // Unlock after checking
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
