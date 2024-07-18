/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:08:49 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 23:47:12 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	handle_single_philosopher(t_philosopher *philo, t_data *data)
{
	print_status(data, philo->id, "has taken a fork");
	accurate_sleep(data->time_to_die);
}

void	introduce_delay(t_philosopher *philo, t_data *data)
{
	if (philo->id % 2 == 0)
		usleep(data->time_to_eat * 500);
}

int	check_simulation_stop(t_data *data)
{
	int		stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->simulation_stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher		*philo;
	int					left_fork;
	int					right_fork;
	t_data				*data;

	philo = (t_philosopher *)arg;
	data = philo->data;
	left_fork = philo->id - 1;
	right_fork = philo->id % data->num_philosophers;
	if (data->num_philosophers == 1)
	{
		handle_single_philosopher(philo, data);
		return (NULL);
	}
	introduce_delay(philo, data);
	while (!check_simulation_stop(data))
	{
		think_and_take_forks(philo, data, left_fork, right_fork);
		eat(philo, data);
		release_forks_and_sleep(philo, data, left_fork, right_fork);
	}
	return (NULL);
}
