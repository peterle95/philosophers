/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:07:27 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 19:29:18 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int init_data(t_data *data, int argc, char **argv)
{
    if (argc < 5 || argc > 6)
    {
        printf("Error: Invalid number of arguments\n");
        return 1;
    }

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
        return 1;
    }

    data->philosophers = malloc(sizeof(t_philosopher) * data->num_philosophers);
    if (!data->philosophers)
    {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philosophers);
    if (!data->forks)
    {
        printf("Error: Memory allocation failed\n");
        free(data->philosophers);
        return 1;
    }

    for (int i = 0; i < data->num_philosophers; i++)
    {
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
        {
            printf("Error: Mutex initialization failed\n");
            // Clean up previously initialized mutexes
            for (int j = 0; j < i; j++)
                pthread_mutex_destroy(&data->forks[j]);
            free(data->forks);
            free(data->philosophers);
            return 1;
        }
    }

    if (pthread_mutex_init(&data->write_lock, NULL) != 0)
    {
        printf("Error: Write lock mutex initialization failed\n");
        for (int i = 0; i < data->num_philosophers; i++)
            pthread_mutex_destroy(&data->forks[i]);
        free(data->forks);
        free(data->philosophers);
        return 1;
    }

    data->start_time = get_current_time();
for (int i = 0; i < data->num_philosophers; i++)
{
    data->philosophers[i].id = i + 1;
    data->philosophers[i].times_eaten = 0;
    data->philosophers[i].last_meal_time = data->start_time;
    data->philosophers[i].data = data;
}

    data->simulation_stop = 0;
    pthread_mutex_init(&data->stop_mutex, NULL);

    return 0;
}