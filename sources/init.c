/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:07:27 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 20:32:03 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	parse_arguments(t_data *data, int argc, char **argv)
{
	data->num_philosophers = atoi(argv[1]);
	data->time_to_die = ft_atoll(argv[2]);
	data->time_to_eat = ft_atoll(argv[3]);
	data->time_to_sleep = ft_atoll(argv[4]);
	data->meals_to_eat = (argc == 6) ? atoi(argv[5]) : -1;
	if (data->num_philosophers < 1 || data->time_to_die < 0 || 
	data->time_to_eat < 0 || data->time_to_sleep < 0 ||
	(argc == 6 && data->meals_to_eat < 0))
	{
		printf("Error: Invalid argument values\n");
		return (1);
	}
	return (0);
}

int	initialize_mutexes(t_data *data)
{
	int		i;
	int		j;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			printf("Error: Mutex initialization failed\n");
			j = 0;
			while (j < i)
			{
				pthread_mutex_destroy(&data->forks[j]);
				j++;
			}
			return (1);
		}
	i++;
	}
	if (pthread_mutex_init(&data->write_lock, NULL) != 0)
	{
		printf("Error: Write lock mutex initialization failed\n");
	for (int i = 0; i < data->num_philosophers; i++)
		pthread_mutex_destroy(&data->forks[i]);
	return (1);
	}
	return (pthread_mutex_init(&data->stop_mutex, NULL));
}

void	initialize_philosophers(t_data *data)
{
	data->start_time = get_current_time();
	for (int i = 0; i < data->num_philosophers; i++)
	{
		data->philosophers[i].id = i + 1;
		data->philosophers[i].times_eaten = 0;
		data->philosophers[i].last_meal_time = data->start_time;		data->philosophers[i].data = data;
	}
	data->simulation_stop = 0;
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (parse_arguments(data, argc, argv) != 0)
		return (1);
	if (allocate_memory(data) != 0)
		return (1);
	if (initialize_mutexes(data) != 0)
	{
		free(data->forks);
		free(data->philosophers);
		return (1);
	}
	initialize_philosophers(data);
	return (0);
}
