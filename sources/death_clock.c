/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_clock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:15:29 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 19:17:07 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool all_philosophers_ate_enough(t_data *data)
{
    if (data->meals_to_eat == -1)
        return false;

    for (int i = 0; i < data->num_philosophers; i++)
    {
        if (data->philosophers[i].times_eaten < data->meals_to_eat)
            return false;
    }
    return true;
}

void *monitor_routine(void *arg)
{
    t_data *data = (t_data *)arg;
    
    if (data->num_philosophers == 1)
    {
        accurate_sleep(data->time_to_die);
        printf("%lld 1 died\n", data->time_to_die);
        data->simulation_stop = 1;
        return NULL;
    }

    while (1)
    {
        for (int i = 0; i < data->num_philosophers; i++)
        {
            if (death_clock(&data->philosophers[i], data))
                return NULL;
        }
        if (all_philosophers_ate_enough(data))
        {
            pthread_mutex_lock(&data->stop_mutex);
            data->simulation_stop = 1;
            pthread_mutex_unlock(&data->stop_mutex);
            printf("All philosophers have eaten enough times\n");
            return NULL;
        }
        usleep(50);  // Check even more frequently
    }
}

bool death_clock(t_philosopher *philo, t_data *data)
{
    long long current_time = get_current_time();
    long long time_since_last_meal = current_time - philo->last_meal_time;

    if (time_since_last_meal > data->time_to_die)
    {
        pthread_mutex_lock(&data->stop_mutex);
        if (!data->simulation_stop)
        {
            data->simulation_stop = 1;
            long long death_time = philo->last_meal_time + data->time_to_die;
            pthread_mutex_unlock(&data->stop_mutex);
            pthread_mutex_lock(&data->write_lock);
            printf("%lld %d died\n", death_time - data->start_time, philo->id);
            pthread_mutex_unlock(&data->write_lock);
            return true;
        }
        pthread_mutex_unlock(&data->stop_mutex);
    }
    return false;
}