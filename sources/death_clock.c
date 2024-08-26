/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_clock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:15:29 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/25 22:29:16 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Handles the case when there's only one philosopher
void	kill_single_philosopher(t_data *data)
{
	accurate_sleep(data->time_to_die);
	printf("%lld 1 died\n", data->time_to_die);
	data->simulation_stop = 1;
}

// Checks if a specific philosopher has died
bool	is_philosopher_dead(t_philosopher *philo, t_data *data)
{
	long	current_time;
	long	time_since_last_meal;
	long	death_time;

	current_time = get_current_time();
	time_since_last_meal = current_time - philo->last_meal_time;
	if (time_since_last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&data->stop_mutex);
		if (data->simulation_stop == 0)
		{
			data->simulation_stop = 1;
			death_time = philo->last_meal_time + data->time_to_die;
			pthread_mutex_unlock(&data->stop_mutex);
			pthread_mutex_lock(&data->write_lock);
			printf("%lld %d died\n", death_time - data->start_time, philo->id);
			pthread_mutex_unlock(&data->write_lock);
			return (true);
		}
		pthread_mutex_unlock(&data->stop_mutex);
	}
	return (false);
}

// Monitors the simulation for philosopher deaths or completion
void	*death_clock_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (data->num_philosophers == 1)
	{
		kill_single_philosopher(data);
		return (NULL);
	}
	while (1)
	{
		if (check_philosopher_deaths(data))
			return (NULL);
		if (check_all_philosophers_ate_enough(data))
			return (NULL);
		usleep(50);
	}
}
