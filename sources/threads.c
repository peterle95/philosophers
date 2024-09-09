/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 23:47:52 by pmolzer           #+#    #+#             */
/*   Updated: 2024/09/10 00:11:40 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Creates threads for each philosopher
int	create_philosopher_threads(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_create(&data->philosophers[i].thread_philo, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
		{
			printf("Error creating thread\n");
			return (1);
		}
		i++;
	}
	return (0);
}

// Creates a thread for the death clock routine
int	create_death_thread(t_data *data, pthread_t *death_clock_thread)
{
	if (pthread_create(death_clock_thread, NULL,
			death_clock_routine, data) != 0)
	{
		printf("Error creating death clock thread\n");
		return (1);
	}
	return (0);
}

// Joins all philosopher threads
int	join_philosopher_threads(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_join(data->philosophers[i].thread_philo, NULL) != 0)
		{
			printf("Error joining thread\n");
			return (1);
		}
		i++;
	}
	return (0);
}

// Creates and runs all threads, including philosophers and death clock
int	create_and_run_threads(t_data *data)
{
	pthread_t	death_clock_thread;

	if (create_philosopher_threads(data) != 0)
		return (1);
	usleep(1000);
	pthread_mutex_lock(&data->stop_mutex); // Lock before accessing shared data
	data->start_time = get_current_time();
	pthread_mutex_unlock(&data->stop_mutex); // Unlock after updating
	if (create_death_thread(data, &death_clock_thread) != 0)
		return (1);
	if (join_philosopher_threads(data) != 0)
		return (1);
	if (pthread_join(death_clock_thread, NULL) != 0)
	{
		printf("Error joining death clock thread\n");
		return (1);
	}
	return (0);
}
