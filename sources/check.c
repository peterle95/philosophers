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

int	check_philosopher_deaths(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (death_clock(&data->philosophers[i], data))
			return (1);
		i++;
	}
	return (0);
}

int	check_all_philosophers_ate_enough(t_data *data)
{
	if (all_philosophers_ate_enough(data))
	{
		set_simulation_stop(data);
		return (1);
	}
	return (0);
}
