/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleenup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:06:00 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 19:06:16 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	cleanup(t_data *data)
{
	int		i;

	i = 0;
	if (data->philosophers)
	{
		free(data->philosophers);
		data->philosophers = NULL;
	}
	if (data->forks)
	{
		while (i < data->num_philosophers)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
		data->forks = NULL;
	}
	pthread_mutex_destroy(&data->write_lock);
}
