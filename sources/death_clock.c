/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_clock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:15:29 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/01 10:58:14 by pmolzer          ###   ########.fr       */
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
			return (false);
		i++;
	}
	return (true);
}

void	handle_single_philosopher_monitor(t_data *data)
{
	accurate_sleep(data->time_to_die);
	printf("%lld 1 died\n", data->time_to_die);
	data->simulation_stop = 1;
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (data->num_philosophers == 1)
	{
		handle_single_philosopher_monitor(data);
		return (NULL);
	}
	while (1)
	{
		if (check_philosopher_deaths(data))
			return (NULL);
		if (check_all_philosophers_ate_enough(data))
			return (NULL);
		usleep(50);  // Check frequently
	}
}

bool	death_clock(t_philosopher *philo, t_data *data)
{
	long	current_time;
	long	time_since_last_meal;
	long	death_time;

	current_time = get_current_time();
	time_since_last_meal = current_time - philo->last_meal_time;
	if (time_since_last_meal > data->time_to_die)
	{
		pthread_mutex_lock(&data->stop_mutex);
		if (!data->simulation_stop)
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
