/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_think_sleep.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 23:44:56 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/25 22:29:38 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Implements a more precise sleep function for accurate timing
void	accurate_sleep(long long time_in_ms)
{
	long long	start;
	long long	end;

	start = get_current_time();
	end = start + time_in_ms;
	while (get_current_time() < end)
	{
		if (end - get_current_time() > 1)
			usleep(500);
		else
			usleep(100);
	}
}

// Handles the thinking phase and fork acquisition for a philosopher
void	think_and_take_forks(t_philosopher *philo, t_data *data,
				int left_fork, int right_fork)
{
	print_status(data, philo->id, "is thinking");
	 if (philo->id % 2 == 0) {
        pthread_mutex_lock(&data->forks[right_fork]);
		print_status(data, philo->id, "has taken a fork");
        pthread_mutex_lock(&data->forks[left_fork]);
		print_status(data, philo->id, "has taken a fork");
    } else {
        pthread_mutex_lock(&data->forks[left_fork]);
		print_status(data, philo->id, "has taken a fork");
        pthread_mutex_lock(&data->forks[right_fork]);
		print_status(data, philo->id, "has taken a fork");
    }
}

// Simulates the eating phase for a philosopher
void	eat(t_philosopher *philo, t_data *data)
{
	philo->last_meal_time = get_current_time();
	print_status(data, philo->id, "is eating");
	accurate_sleep(data->time_to_eat);
	philo->times_eaten++;
}

// Handles the release of forks and the sleeping phase for a philosopher
void	release_forks_and_sleep(t_philosopher *philo, t_data *data,
				int left_fork, int right_fork)
{
	pthread_mutex_unlock(&data->forks[left_fork]);
	pthread_mutex_unlock(&data->forks[right_fork]);
	print_status(data, philo->id, "is sleeping");
	accurate_sleep(data->time_to_sleep);
}
