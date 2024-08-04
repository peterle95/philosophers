/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 23:50:04 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 23:50:49 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	all_philosophers_ate_enough(t_data *data)
{
	int		i;

	i = 0;
	if (data->meals_to_eat == -1)
		return (false);
	while (i < data->num_philosophers)
	{
		if (data->philosophers[i].times_eaten < data->meals_to_eat)
		/*Check for every philo has eaten enough*/
			return (false);
		i++;
	}
	return (true);
}

int	check_philosopher_deaths(t_data *data)
/*It iterates through all philosophers in the simulation.
For each philosopher, it calls is_philosopher_dead.
If any philosopher is found to be dead, it immediately returns 1 (true).
If no philosopher is dead, it returns 0 (false).*/
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

int	check_all_philosophers_ate_enough(t_data *data)
{
	if (all_philosophers_ate_enough(data))
	/*If the function call is true, then enter*/
	{
		set_simulation_stop(data);
		return (1);
	}
	return (0);
}
