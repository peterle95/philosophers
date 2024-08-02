/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 23:47:52 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/02 14:56:20 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	create_philosopher_threads(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_create(&data->philosophers[i].thread, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
				/*
				The cast in the philosopher_routine function is still necessary. Here's why:
					pthread_create expects a function of type void *(*start_routine) (void *).
					When the thread starts, it calls this function with a void * argument.
					Inside philosopher_routine, you need to cast this void * back to t_philosopher
				
				&data->philosophers[i] is passed as the last argument. This is a pointer to the t_philosopher struct for the i-th philosopher.*/
		{
			printf("Error creating thread\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	create_death_thread(t_data *data, pthread_t *death_clock_thread)
{
	if (pthread_create(death_clock_thread, NULL, death_clock_routine, data) != 0)
	/*Go in depths how this function works*/
	{
		printf("Error creating death clock thread\n");
		return (1);
	}
	return (0);
}

int	join_philosopher_threads(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_join(data->philosophers[i].thread, NULL) != 0)
		{
			printf("Error joining thread\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	create_and_run_threads(t_data *data)
{
	pthread_t	death_clock_thread;

	if (create_philosopher_threads(data) != 0)
		return (1);
	usleep(1000);
	/*It introduces a small delay to allow all philosopher threads to be created and initialized before the simulation officially starts.
	It helps to reduce the chance of race conditions that might occur if the start time is set immediately after thread creation.
	It can help distribute the initial actions of the philosophers, preventing 
		them from all starting simultaneously, which could lead to immediate deadlocks in some implementations.*/
	data->start_time = get_current_time();
	// why is there here and in the "void	initialize_philosophers(t_data *data)", start_time fetching?
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
