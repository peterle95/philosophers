/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:08:49 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 19:09:28 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;
    int left_fork = philo->id - 1;
    int right_fork = philo->id % data->num_philosophers;

    if (data->num_philosophers == 1)
    {
        print_status(data, philo->id, "has taken a fork");
        accurate_sleep(data->time_to_die);
        return NULL;
    }

    // Introduce a more significant delay for even-numbered philosophers
    if (philo->id % 2 == 0)
        usleep(data->time_to_eat * 500);  // Half of time_to_eat in microseconds

    while (1)
    {
        pthread_mutex_lock(&data->stop_mutex);
        if (data->simulation_stop)
        {
            pthread_mutex_unlock(&data->stop_mutex);
            break;
        }
        pthread_mutex_unlock(&data->stop_mutex);

        // Think
        print_status(data, philo->id, "is thinking");
        
        // Pick up forks
        pthread_mutex_lock(&data->forks[left_fork]);
        print_status(data, philo->id, "has taken a fork");
        pthread_mutex_lock(&data->forks[right_fork]);
        print_status(data, philo->id, "has taken a fork");
        
        // Eat
        philo->last_meal_time = get_current_time();
        print_status(data, philo->id, "is eating");
        accurate_sleep(data->time_to_eat);
        philo->times_eaten++;

        // Put down forks
        pthread_mutex_unlock(&data->forks[left_fork]);
        pthread_mutex_unlock(&data->forks[right_fork]);
        
        // Sleep
        print_status(data, philo->id, "is sleeping");
        accurate_sleep(data->time_to_sleep);
    }

    return NULL;
}